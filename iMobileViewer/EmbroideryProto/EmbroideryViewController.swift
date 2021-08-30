import UIKit

class EmbroideryViewController: UIViewController {
    
    
    @IBOutlet weak var txtFile: UITextField!
    @IBOutlet weak var stitchView: StitchView! {
        didSet {
            stitchView.addGestureRecognizer(UIPinchGestureRecognizer(
                target: stitchView,
                action: #selector(StitchView.changeScale(_:))
            ))
        }
    }
    
    
    func loadFile(_ theurl : URL) {
        BinaryHelper.load(url: theurl,
                          completion: {file in
                            var pattern : EmbPattern
                            let fileextension = String(file.filename.suffix(3)).lowercased()
                            var reader : FormatProtocol? = nil
                            if fileextension == "exp"  {
                                reader = FormatExp()
                            } else if fileextension == "pcs" {
                                reader = FormatPcs()
                            } else if fileextension == "pec" {
                                reader = FormatPec()
                            } else if fileextension == "pes" {
                                reader = FormatPes()
                            }
                            if reader != nil {
                                pattern = (reader?.read(file: file))!
                            } else {
                                pattern = EmbPattern()
                            }
                            self.stitchView.setPattern(pattern)
                            DispatchQueue.main.async {
                                self.stitchView.setNeedsDisplay()
                            }
        })
    }
    
    @IBAction func btnDraw(_ sender: UIButton) {
        loadFile(URL(string: self.txtFile.text!)!)
    }
}

