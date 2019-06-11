/* Copyright © 2019 Apple Inc. All rights reserved.
 *
 * Use of this source code is governed by a BSD-3-clause license that can
 * be found in the LICENSE.txt file or at https://opensource.org/licenses/BSD-3-Clause
 */

const css = require('../../external/javascript/css-2.2.4');

const assert = require('assert');
const fs = require('fs');
const path = require('path');

if (process.argv.length < 6) {
    console.log(`css_module_linker.js usage:
node css_module_linker.js <target_sentinel_file> <base_source_dir> <base_build_dir> <object_file> [<object_file>] [<object_file>] [...]

Expects object files produced by css_module_compiler.js. Writes out:

<target_sentinel_file> (an empty file with timestamp for build system tracking),
<base_build_dir>/index.css (the combined .css output), and
<base_build_dir>/<object_path>/<object_file_basename>.json
(where target_dir is the directory containing target_file,
 object_path is the directory of object_file relative to base_source_dir,
 and <object_file_basename> is the base filename (minus extension) of object_file.)
    `);
    process.exit(1);
}

assert(process.argv[0].endsWith('node'));
assert(process.argv[1].endsWith('css_module_linker.js'));
const targetSentinelFile = process.argv[2];
const baseSourceDir = process.argv[3];
const baseBuildDir = process.argv[4];
const targetCSSFile = path.join(baseBuildDir, path.basename(targetSentinelFile, path.extname(targetSentinelFile)) + '.css');
const objectFiles = process.argv.slice(5);

const outputCSS = [];
for (const objectFile of objectFiles) {
    const object = JSON.parse(fs.readFileSync(objectFile));
    const parsed = object.css;
    outputCSS.push(css.stringify(parsed, { compress: true }));

    const sourceFile = object.css.stylesheet.source;
    const outputPath = appendExt(path.join(baseBuildDir, path.relative(baseSourceDir, sourceFile)), "json");
    fs.mkdirSync(path.dirname(outputPath), {recursive: true});
    fs.writeFileSync(outputPath, JSON.stringify(object.json));
}

fs.writeFileSync(targetCSSFile, outputCSS.join());
fs.writeFileSync(targetSentinelFile, "");

function appendExt(filePath, newExt) {
    return filePath + '.' + newExt;
}