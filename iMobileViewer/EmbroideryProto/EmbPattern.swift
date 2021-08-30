import UIKit

public class EmbPattern {
    public var blocks = [StitchBlock]()
    private var currentPoint = CGPoint()
    private var currentBlock = StitchBlock()
    private var threadList = [UIColor]()
    
    func addThread(_ thread : UIColor) {
        threadList.append(thread)
    }
    
    func addStitchRel(point: CGPoint, stitchType: StitchType) {
        let tempPoint = CGPoint(x: point.x + currentPoint.x, y: point.y + currentPoint.y)
        self.addStitchAbs(point: tempPoint, stitchType: stitchType)
    }
    
    func addStitchAbs(point: CGPoint, stitchType: StitchType) {
        if blocks.count == 0 {
            if(threadList.count == 0) {
                let newThread = UIColor(red: CGFloat(Float(arc4random()) / Float(UINT32_MAX)), green: CGFloat(Float(arc4random()) / Float(UINT32_MAX)), blue:CGFloat(Float(arc4random()) / Float(UINT32_MAX)), alpha: 1.0)
                self.threadList.append(newThread)
                currentBlock.Color = newThread
            } else{
                currentBlock.Color = self.threadList[0]
            }
            blocks.append(currentBlock)
        }
        if stitchType == StitchType.End {
            if self.currentBlock.Points.count == 0 {
                self.blocks.remove(at: self.blocks.count - 1)
                return
            }
        }
        if (stitchType.rawValue & StitchType.Stop.rawValue) > 0 {
            if self.currentBlock.Points.count == 0 {
                return
            }
            var threadIndex = 0
            let currIndex = self.threadList.firstIndex(of: self.currentBlock.Color)
            if currIndex == nil || (currIndex! + 1) >= self.threadList.count {
                    let newThread = UIColor(red: CGFloat(Float(arc4random()) / Float(UINT32_MAX)), green: CGFloat(Float(arc4random()) / Float(UINT32_MAX)), blue:CGFloat(Float(arc4random()) / Float(UINT32_MAX)), alpha: 1.0)
                    self.threadList.append(newThread)
                }
            if currIndex == nil {
                threadIndex = 0
            } else {
                threadIndex = currIndex! + 1
            }
            let sb = StitchBlock()
            self.currentBlock = sb
            sb.Color = self.threadList[threadIndex]
            self.blocks.append(sb)
            return
        }
        if (stitchType.rawValue & StitchType.Trim.rawValue) > 0 {
            currentPoint = point
            if self.currentBlock.Points.count == 0 {
                return
            }
            let currIndex = self.threadList.firstIndex(of: self.currentBlock.Color)
            let sb = StitchBlock()
            self.currentBlock = sb
            sb.Color = self.threadList[currIndex!]
            self.blocks.append(sb)
            return
        }
        currentPoint = point
        if (stitchType.rawValue & StitchType.Jump.rawValue) == 0 {
            self.currentBlock.Points.append(point)
        }
    }
    
    func calculateBoundingBox() -> CGRect {
        var left = CGFloat.infinity
        var top = CGFloat.infinity
        var right = -CGFloat.infinity
        var bottom = -CGFloat.infinity
        for sb in self.blocks {
            let dim = sb.getBounds()
            left = min(left, dim.minX)
            top = min(top, dim.minY)
            right = max(right, dim.maxX)
            bottom = max(bottom, dim.maxY)
        }
        return CGRect(x: left, y: top, width: right - left, height: bottom - top)
    }
}
