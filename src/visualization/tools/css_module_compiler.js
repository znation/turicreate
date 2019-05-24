/* Copyright © 2019 Apple Inc. All rights reserved.
 *
 * Use of this source code is governed by a BSD-3-clause license that can
 * be found in the LICENSE.txt file or at https://opensource.org/licenses/BSD-3-Clause
 */

const css = require('../../external/javascript/css-2.2.4');
const cssWhat = require('../../external/javascript/css-what-2.1.3');
const cssWhatStringify = require('../../external/javascript/css-what-2.1.3/stringify.js');

const assert = require('assert');
const child_process = require('child_process');
const fs = require('fs');
const path = require('path');

if (process.argv.length != 4) {
    console.log(`css_module_compiler.js usage:
node css_module_compiler.js <input_css_module> <output_object_file>

Writes a JSON object describing the CSS, and a mapping of original to unique class names.

Note that input_css_module can be either .css (no preprocessor) or .scss (preprocessed with sassc).
    `);
    process.exit(1);
}

assert(process.argv[0].endsWith('node'));
assert(process.argv[1].endsWith('css_module_compiler.js'));
const inputFile = process.argv[2];
const outputFile = process.argv[3];
let input = String(fs.readFileSync(inputFile));

// If the filename ends in .scss, run through sassc compiler first
if (inputFile.endsWith('.scss')) {
    input = compileSCSS(input);
}

const parsed = css.parse(input, { source: inputFile });
const classNameMap = {};

for (const rule of parsed.stylesheet.rules) {
    if (rule.type == 'comment' ||
        rule.type == 'keyframes') {
        continue;
    }
    for (let i = 0; i < rule.selectors.length; i++) {
        const selector = rule.selectors[i];
        const parsedSelector = cssWhat(selector);
        // since both css and cssWhat take care of comma-parsing multiple selectors,
        // we should expect parsedSelector to have only found one
        assert(parsedSelector.length == 1);
        for (const element of parsedSelector[0]) {
            if (element.type == 'attribute' && element.name == 'class') {
                assert(element.action == 'element'); // not sure what else this could be
                const previousValue = element.value;
                element.value = makeUniqueID() + '_' + previousValue;
                classNameMap[previousValue] = element.value;
            } 
        }
        rule.selectors[i] = cssWhatStringify(parsedSelector);
    }
}

const output = {
    "css": parsed,
    "json": classNameMap
};

console.log("Writing CSS object file to ", outputFile);
fs.writeFileSync(outputFile, JSON.stringify(output));

function makeUniqueID() {
    return Math.random().toString(36).substring(2, 15) + Math.random().toString(36).substring(2, 15);
}

function compileSCSS(str) {
    return child_process.execSync(path.join(__dirname, '../../../deps/local/bin/sassc'), {input: str}).toString('utf8');
}