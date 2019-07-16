//  Copyright © 2019 Apple Inc. All rights reserved.
//
//  Use of this source code is governed by a BSD-3-clause license that can
//  be found in the LICENSE.txt file or at https://opensource.org/licenses/BSD-3-Clause

import Cocoa

let args = CommandLine.arguments

if args.count != 3 || args[1] != "--server" {
    let msg = """
        Turi Create Visualization usage:

        --server <HTTP_SERVER_URL> (required)

        The HTTP server root should point to the static frontend assets for Turi Create Visualization.

        """
    fputs(msg, stderr)
    fputs("\n", stderr)
    exit(1)
}


guard let server = URL(string: args[2]) else {
    fputs("Error: server \(args[2]) is not a valid URL.\n", stderr)
    exit(1)
}

let delegate = AppDelegate(server: server)
NSApplication.shared.delegate = delegate
NSApplication.shared.run()
