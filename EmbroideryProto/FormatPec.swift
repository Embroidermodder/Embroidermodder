import UIKit

class FormatPec : FormatProtocol {
    
    private static var pecThreads = [UIColor]()
    class func getPecThreads() -> [UIColor] {
        if pecThreads.count == 0 {
            pecThreads.append(UIColor.init(rgbColorCodeRed:   0, green : 0, blue :    0))
            pecThreads.append(UIColor.init(rgbColorCodeRed:  14, green : 31, blue :  124))
            pecThreads.append(UIColor.init(rgbColorCodeRed:  10, green : 85, blue :  163))
            pecThreads.append(UIColor.init(rgbColorCodeRed:   0, green : 135, blue :  119))
            pecThreads.append(UIColor.init(rgbColorCodeRed:  75, green : 107, blue :  175))
            pecThreads.append(UIColor.init(rgbColorCodeRed: 237, green : 23, blue :   31))
            pecThreads.append(UIColor.init(rgbColorCodeRed: 209, green : 92, blue :    0))
            pecThreads.append(UIColor.init(rgbColorCodeRed: 145, green : 54, blue :  151))
            pecThreads.append(UIColor.init(rgbColorCodeRed: 228, green : 154, blue :  203))
            pecThreads.append(UIColor.init(rgbColorCodeRed: 145, green : 95, blue :  172))
            pecThreads.append(UIColor.init(rgbColorCodeRed: 158, green : 214, blue :  125))
            pecThreads.append(UIColor.init(rgbColorCodeRed: 232, green : 169, blue :    0))
            pecThreads.append(UIColor.init(rgbColorCodeRed: 254, green : 186, blue :   53))
            pecThreads.append(UIColor.init(rgbColorCodeRed: 255, green : 255, blue :    0))
            pecThreads.append(UIColor.init(rgbColorCodeRed: 112, green : 188, blue :   31))
            pecThreads.append(UIColor.init(rgbColorCodeRed: 186, green : 152, blue :    0))
            pecThreads.append(UIColor.init(rgbColorCodeRed: 168, green : 168, blue :  168))
            pecThreads.append(UIColor.init(rgbColorCodeRed: 125, green : 111, blue :    0))
            pecThreads.append(UIColor.init(rgbColorCodeRed: 255, green : 255, blue :  179))
            pecThreads.append(UIColor.init(rgbColorCodeRed:  79, green : 85, blue :   86))
            pecThreads.append(UIColor.init(rgbColorCodeRed:   0, green : 0, blue :    0))
            pecThreads.append(UIColor.init(rgbColorCodeRed:  11, green : 61, blue :  145))
            pecThreads.append(UIColor.init(rgbColorCodeRed: 119, green : 1, blue :  118))
            pecThreads.append(UIColor.init(rgbColorCodeRed:  41, green : 49, blue :   51))
            pecThreads.append(UIColor.init(rgbColorCodeRed:  42, green : 19, blue :    1))
            pecThreads.append(UIColor.init(rgbColorCodeRed: 246, green : 74, blue :  138))
            pecThreads.append(UIColor.init(rgbColorCodeRed: 178, green : 118, blue :   36))
            pecThreads.append(UIColor.init(rgbColorCodeRed: 252, green : 187, blue :  197))
            pecThreads.append(UIColor.init(rgbColorCodeRed: 254, green : 55, blue :   15))
            pecThreads.append(UIColor.init(rgbColorCodeRed: 240, green : 240, blue :  240))
            pecThreads.append(UIColor.init(rgbColorCodeRed: 106, green : 28, blue :  138))
            pecThreads.append(UIColor.init(rgbColorCodeRed: 168, green : 221, blue :  196))
            pecThreads.append(UIColor.init(rgbColorCodeRed:  37, green : 132, blue :  187))
            pecThreads.append(UIColor.init(rgbColorCodeRed: 254, green : 179, blue :   67))
            pecThreads.append(UIColor.init(rgbColorCodeRed: 255, green : 243, blue :  107))
            pecThreads.append(UIColor.init(rgbColorCodeRed: 208, green : 166, blue :   96))
            pecThreads.append(UIColor.init(rgbColorCodeRed: 209, green : 84, blue :    0))
            pecThreads.append(UIColor.init(rgbColorCodeRed: 102, green : 186, blue :   73))
            pecThreads.append(UIColor.init(rgbColorCodeRed:  19, green : 74, blue :   70))
            pecThreads.append(UIColor.init(rgbColorCodeRed: 135, green : 135, blue :  135))
            pecThreads.append(UIColor.init(rgbColorCodeRed: 216, green : 204, blue :  198))
            pecThreads.append(UIColor.init(rgbColorCodeRed:  67, green : 86, blue :    7))
            pecThreads.append(UIColor.init(rgbColorCodeRed: 253, green : 217, blue :  222))
            pecThreads.append(UIColor.init(rgbColorCodeRed: 249, green : 147, blue :  188))
            pecThreads.append(UIColor.init(rgbColorCodeRed:   0, green : 56, blue :   34))
            pecThreads.append(UIColor.init(rgbColorCodeRed: 178, green : 175, blue :  212))
            pecThreads.append(UIColor.init(rgbColorCodeRed: 104, green : 106, blue :  176))
            pecThreads.append(UIColor.init(rgbColorCodeRed: 239, green : 227, blue :  185))
            pecThreads.append(UIColor.init(rgbColorCodeRed: 247, green : 56, blue :  102))
            pecThreads.append(UIColor.init(rgbColorCodeRed: 181, green : 75, blue :  100))
            pecThreads.append(UIColor.init(rgbColorCodeRed:  19, green : 43, blue :   26))
            pecThreads.append(UIColor.init(rgbColorCodeRed: 199, green : 1, blue :   86))
            pecThreads.append(UIColor.init(rgbColorCodeRed: 254, green : 158, blue :   50))
            pecThreads.append(UIColor.init(rgbColorCodeRed: 168, green : 222, blue :  235))
            pecThreads.append(UIColor.init(rgbColorCodeRed:   0, green : 103, blue :   62))
            pecThreads.append(UIColor.init(rgbColorCodeRed:  78, green : 41, blue :  144))
            pecThreads.append(UIColor.init(rgbColorCodeRed:  47, green : 126, blue :   32))
            pecThreads.append(UIColor.init(rgbColorCodeRed: 255, green : 204, blue :  204))
            pecThreads.append(UIColor.init(rgbColorCodeRed: 255, green : 217, blue :   17))
            pecThreads.append(UIColor.init(rgbColorCodeRed:   9, green : 91, blue :  166))
            pecThreads.append(UIColor.init(rgbColorCodeRed: 240, green : 249, blue :  112))
            pecThreads.append(UIColor.init(rgbColorCodeRed: 227, green : 243, blue :   91))
            pecThreads.append(UIColor.init(rgbColorCodeRed: 255, green : 153, blue :    0))
            pecThreads.append(UIColor.init(rgbColorCodeRed: 255, green : 240, blue :  141))
            pecThreads.append(UIColor.init(rgbColorCodeRed: 255, green : 200, blue :  200))
        }
        return pecThreads
    }
    class func readPecStitches(pattern : EmbPattern, file : InMemoryFile) {
    var stitchNumber = 0

    while(file.offsetInFile < file.count()) {
        var val1 = Int(file.readUInt8())
        var val2 = Int(file.readUInt8())
        
        var stitchType = StitchType.Normal
        if val1 == 0xFF && val2 == 0x00 {
            pattern.addStitchRel(point: CGPoint.zero, stitchType: StitchType.End)
            break
        }
        if(val1 == 0xFE && val2 == 0xB0)
        {
            file.readUInt8()
            pattern.addStitchRel(point: CGPoint.zero, stitchType: StitchType.Stop)
            stitchNumber += 1
            continue
        }
        /* High bit set means 12-bit offset, otherwise 7-bit signed delta */
        if (val1 & 0x80) > 0 {
            if(val1 & 0x20) > 0 {
                stitchType = StitchType.Trim
            }
            if(val1 & 0x10) > 0 {
                stitchType = StitchType.Jump
            }
            val1 = ((val1 & 0x0F) << 8) + val2
            
            /* Signed 12-bit arithmetic */
            if(val1 & 0x800) > 0 {
                val1 -= 0x1000
            }
            
            val2 = Int(file.readUInt8())
        }
        else if(val1 >= 0x40)
        {
            val1 -= 0x80;
        }
        if(val2 & 0x80) > 0 {
            if(val2 & 0x20) > 0 {
            stitchType = StitchType.Trim
            }
            if(val2 & 0x10) > 0 {
            stitchType = StitchType.Jump
            }
            val2 = ((val2 & 0x0F) << 8) + Int(file.readUInt8())
            
            /* Signed 12-bit arithmetic */
            if(val2 & 0x800) > 0 {
                val2 -= 0x1000
            }
        }
        else if(val2 >= 0x40)
        {
            val2 -= 0x80
        }
        pattern.addStitchRel(point: CGPoint(x: CGFloat(val1), y: -CGFloat(val2)), stitchType: stitchType)
        stitchNumber += 1
    }
}

func read(file : InMemoryFile) -> EmbPattern {
    let pattern = EmbPattern()
    file.seek(set: 0x38)
    let colorChanges = file.readUInt8()
    for _ in 0...colorChanges {
        pattern.addThread(FormatPec.getPecThreads()[Int(file.readUInt8() % 65)])
    }
    file.seek(set: 0x21C)
    FormatPec.readPecStitches(pattern: pattern, file: file)
    return pattern
}
}
