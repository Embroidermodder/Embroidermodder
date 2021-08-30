import UIKit
class FormatPcs : FormatProtocol {
    class func pcsDecode(a1 : UInt8, a2 : UInt8, a3 : UInt8) -> Int32 {
        var res : Int32
        res = Int32(a1) | Int32(a2) << 8 | Int32(a3) << 16
        if (res > 0x7FFFFF) {
            return (-((~(res) & 0x7FFFFF) - 1))
        }
        return res;
    }
    
    func read(file : InMemoryFile) -> EmbPattern {
        var allZeroColor = true
        var b = [UInt8]()
        var dx : Int32
        var dy : Int32
        var flags : StitchType
        var st : Int16
        var version : UInt8
        var hoopSize : UInt8
        var colorCount : Int16
        let pattern = EmbPattern()
        version = file.readUInt8()
        hoopSize = file.readUInt8()  /* 0 for PCD, 1 for PCQ (MAXI), 2 for PCS with small hoop(80x80), */
        /* and 3 for PCS with large hoop (115x120) */
        colorCount = file.readInt16LE()
        for _ in 1...colorCount {
            let red = file.readUInt8()
            let green = file.readUInt8()
            let blue = file.readUInt8()
            let t = UIColor(red: CGFloat(red) / CGFloat(255), green: CGFloat(green) / 255, blue: CGFloat(blue) / 255, alpha: 1.0)
            if (red != 0 || green != 0 || blue != 0) {
                allZeroColor = false
            }
            pattern.addThread(t);
            _ = file.readUInt8()
        }
        st = file.readInt16LE()
        for _ in 1...st {
            flags = StitchType.Normal
            b = file.readBytes(length: 9)
            if b.count < 9 {
                break;
            }
            if ((b[8] & 0x01) != 0) {
                flags = StitchType.Stop
            } else if ((b[8] & 0x04) != 0) {
                flags = StitchType.Trim
            }
            dx = FormatPcs.pcsDecode(a1: b[1], a2: b[2], a3: b[3])
            dy = FormatPcs.pcsDecode(a1: b[5], a2: b[6], a3: b[7])
            pattern.addStitchAbs(point: CGPoint(x: CGFloat(dx), y: CGFloat(dy)), stitchType: flags)
        }
        
        //pattern.getFlippedPattern(false, true);
        pattern.addStitchRel(point: CGPoint.zero, stitchType: StitchType.End)
        return pattern
    }
}
