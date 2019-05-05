//  Copyright © 2017 Apple Inc. All rights reserved.
//
//  Use of this source code is governed by a BSD-3-clause license that can
//  be found in the LICENSE.txt file or at https://opensource.org/licenses/BSD-3-Clause
import Cocoa

@NSApplicationMain
class AppDelegate: NSObject, NSApplicationDelegate {
    
    var save_image_object: NSMenuItem
    var save_vega_object: NSMenuItem
    var print_vega_image: NSMenuItem
    var page_setup_object: NSMenuItem
    
    override init() {
        save_image_object = NSMenuItem(title: "Export as PNG", action: #selector(self.save_image), keyEquivalent: "S")
        save_image_object.keyEquivalentModifierMask = .command
        
        save_vega_object = NSMenuItem(title: "Export as Vega JSON", action: #selector(self.save_vega), keyEquivalent: "S")
        save_vega_object.keyEquivalentModifierMask = NSEvent.ModifierFlags(rawValue: NSEvent.ModifierFlags.command.rawValue | NSEvent.ModifierFlags.shift.rawValue)
        
        print_vega_image = NSMenuItem(title: "Print...", action: #selector(self.print_vega), keyEquivalent: "P")
        print_vega_image.keyEquivalentModifierMask = .command
        
        page_setup_object = NSMenuItem(title: "Page Setup...", action: #selector(self.print_vega), keyEquivalent: "P")
        page_setup_object.keyEquivalentModifierMask = NSEvent.ModifierFlags(rawValue: NSEvent.ModifierFlags.command.rawValue | NSEvent.ModifierFlags.shift.rawValue)

    }
    
    @objc func print_vega(_ sender: Any) {
        SharedData.shared.vegaContainer?.get_image {image in
            let nsImage = NSImageView(image: image)
            nsImage.sizeToFit()
            nsImage.setFrameOrigin(NSPoint(x: 0.0, y: 0.0))
            nsImage.setFrameSize(NSSize(width: image.size.width, height: image.size.height))
            let print_info = NSPrintInfo()
        
            let print_image = NSPrintOperation(view: nsImage, printInfo: print_info)
            print_image.canSpawnSeparateThread = true
            print_image.printPanel.options.insert(NSPrintPanel.Options.showsOrientation)
            print_image.printPanel.options.insert(NSPrintPanel.Options.showsScaling)
            print_image.run()
        }
    }
    
    @objc func save_image(_ sender: Any) {
        SharedData.shared.vegaContainer?.save_image()
    }
    
    @objc func save_vega(_ sender: Any) {
        SharedData.shared.vegaContainer?.save_vega()
    }
    
    func save_data(_ sender: Any) {
        SharedData.shared.vegaContainer?.save_data()
    }
    
    
    func applicationDidFinishLaunching(_ aNotification: Notification) {
        SharedData.shared.save_image = save_image_object
        SharedData.shared.save_vega = save_vega_object
        SharedData.shared.print_vega = print_vega_image
        SharedData.shared.page_setup = page_setup_object
    }

    func applicationWillTerminate(_ aNotification: Notification) {
        // Insert code here to tear down your application
    }
}

