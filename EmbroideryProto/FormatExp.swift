import UIKit

class FormatExp : FormatProtocol {
    func read(file : InMemoryFile) -> EmbPattern {
        let pattern = EmbPattern()
        let fileLength = file.count()
        while file.offsetInFile < fileLength {
            var b0 = file.readInt8()
            var b1 = file.readInt8()
            var flags: StitchType = StitchType.Normal
            if (b0 == -128) {
                if ((b1 & 1) > 0) {
                    b0 = file.readInt8()
                    b1 = file.readInt8()
                    flags = StitchType.Stop
                } else if ((b1 == 2) || (b1 == 4) || b1 == 6) {
                    flags = StitchType.Trim
                    if (b1 == 2) {
                        flags = StitchType.Normal
                    }
                    b0 = file.readInt8()
                    b1 = file.readInt8()
                } else if (b1 == -128) {
                    b0 = file.readInt8()
                    b1 = file.readInt8()
                    b0 = 0
                    b1 = 0
                    flags = StitchType.Trim
                }
            }
            pattern.addStitchRel(point: CGPoint(x: CGFloat(b0), y: CGFloat(b1)), stitchType: flags)
        }
        pattern.addStitchRel(point: CGPoint.zero, stitchType: StitchType.End)
        return pattern
    }
}
