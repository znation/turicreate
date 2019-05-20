/* Copyright © 2019 Apple Inc. All rights reserved.
 *
 * Use of this source code is governed by a BSD-3-clause license that can
 * be found in the LICENSE.txt file or at https://opensource.org/licenses/BSD-3-Clause
 */

const css = require('../../external/javascript/css-2.2.4');

const assert = require('assert');
const fs = require('fs');
const path = require('path');

if (process.argv.length < 4) {
    console.log(`css_module_linker.js usage:
node css_module_linker.js <target_path> <object_file> [<object_file>] [<object_file>] [...]

Expects object files produced by css_module_compiler.js.
    `);
    process.exit(1);
}

assert(process.argv[0].endsWith('node'));
assert(process.argv[1].endsWith('css_module_compiler.js'));
const targetPath = process.argv[2];
const objectFiles = process.argv.slice(3);

const outputCSS = [];
for (const objectFile of objectFiles) {
    const object = JSON.parse(fs.readFileSync(objectFile));
    const parsed = object.css;
    outputCSS.push(css.stringify(parsed, { compress: true }));

    const classNameMap = object.json;
    const outputJSON = JSON.stringify(classNameMap);

    console.error(objectFile);
    process.exit(1);
    // TODO write the JSON out in something like the original path... ?
}

