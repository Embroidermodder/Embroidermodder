import UIKit

class StitchView: UIView {
    public var pattern : EmbPattern = EmbPattern()

    public func setPattern(_ pattern: EmbPattern) {
        self.pattern = pattern
        self.backgroundColor = UIColor(red: 0.9, green:0.9, blue: 0.9, alpha: 1.0)
    }
    
    var scale = CGFloat(1.0)

    @objc func changeScale(_ recognizer : UIPinchGestureRecognizer) {
        switch recognizer.state {
        case .changed, .ended :
            scale *= recognizer.scale
            self.transform = CGAffineTransform(scaleX: scale, y: scale)
            recognizer.scale = 1
        default :
            break
        }
    }

    func drawStitches() {
        let context = UIGraphicsGetCurrentContext()
        // scale and translate to the standard cartesian coordinate system where the (0,0) is the center of the screen.
        context!.scaleBy(x: 1, y: -1);
        let bounds = self.pattern.calculateBoundingBox()
        
        let widthRatio = bounds.width / self.bounds.size.width
        let heightRatio = bounds.height / self.bounds.size.height
        let scaleRatio = max(widthRatio, heightRatio)
        context!.translateBy(x: self.bounds.size.width*0.5 - (bounds.midX / scaleRatio), y: -(self.bounds.size.height*0.5 + (bounds.midY / scaleRatio)))
        
        for stitch in self.pattern.blocks {
            let path = UIBezierPath()
            path.lineJoinStyle = .round
            var isFirst = true
            stitch.Color.setStroke()
            for item in stitch.Points {
                if isFirst {
                    path.move(to: CGPoint(x: item.x / scaleRatio, y: item.y / scaleRatio))
                    isFirst = false
                }
                else {
                    path.addLine(to: CGPoint(x: item.x / scaleRatio, y: item.y / scaleRatio))
                }
            }
            path.stroke()
        }
    }

    override func draw(_ rect: CGRect) {
        self.drawStitches()
    }

}
