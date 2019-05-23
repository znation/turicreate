//  Copyright © 2017 Apple Inc. All rights reserved.
//
//  Use of this source code is governed by a BSD-3-clause license that can
//  be found in the LICENSE.txt file or at https://opensource.org/licenses/BSD-3-Clause

import Cocoa
import WebKit

class CustomWebKitView: WKWebView {
    override func performKeyEquivalent(with event: NSEvent) -> Bool {
        let ret = super.performKeyEquivalent(with: event)
        if (event.modifierFlags.contains(.command) &&
            (event.charactersIgnoringModifiers == "=" ||
                event.charactersIgnoringModifiers == "-" ||
                event.charactersIgnoringModifiers == "+")) {
            // if cmd+ or cmd- are pressed, those will zoom in the
            // browser pane and are handled.
            return true
        }
        return ret
    }
}
