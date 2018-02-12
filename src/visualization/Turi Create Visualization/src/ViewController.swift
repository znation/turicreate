//  Copyright © 2017 Apple Inc. All rights reserved.
//
//  Use of this source code is governed by a BSD-3-clause license that can
//  be found in the LICENSE.txt file or at https://opensource.org/licenses/BSD-3-Clause

import WebKit

class ViewController: NSViewController, NSWindowDelegate {
    
    //@IBOutlet weak var view: NSView!
    
    // instance of WKWebView
    let webView = WKWebView()
    
    // load view
    override func viewDidLoad() {
        super.viewDidLoad()
        
        webView.focusRingType = NSFocusRingType.exterior
        webView.translatesAutoresizingMaskIntoConstraints = false
        webView.autoresizingMask.update(with: NSAutoresizingMaskOptions.viewWidthSizable.union(NSAutoresizingMaskOptions.viewHeightSizable))
        
        // add the WKWebView to the "holder" UIView
        view.addSubview(webView)
        
        // pin to all 4 edges
        /*
        webView.topAnchor.constraint(equalTo: view.topAnchor, constant: 0.0).isActive = true
        webView.bottomAnchor.constraint(equalTo: view.bottomAnchor, constant: 0.0).isActive = true
        webView.leadingAnchor.constraint(equalTo: view.leadingAnchor, constant: 0.0).isActive = true
        webView.trailingAnchor.constraint(equalTo: view.trailingAnchor, constant: 0.0).isActive = true
 */
        
        // create graph object
        SharedData.shared.vegaContainer = VegaContainer(view: webView)
    }
    
    override func viewDidAppear() {
        super.viewDidAppear()
        
        self.view.window?.delegate = self
        self.view.layer?.backgroundColor = NSColor.white.cgColor
    }
    
    func windowShouldClose(_ sender: Any) -> Bool {
        NSApplication.shared().terminate(self)
        return true
    }

}



