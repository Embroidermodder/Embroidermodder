import UIKit
extension UIColor {
    
    convenience init(rgbColorCodeRed red: Int, green: Int, blue: Int) {
        
        let redPart: CGFloat = CGFloat(red) / 255
        let greenPart: CGFloat = CGFloat(green) / 255
        let bluePart: CGFloat = CGFloat(blue) / 255
        
        self.init(red: redPart, green: greenPart, blue: bluePart, alpha: 1.0)
        
    }
}
