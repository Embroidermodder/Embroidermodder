import Foundation

class InMemoryFile {
    public var offsetInFile : Int = 0
    private var data: Data
    public var filename : String = ""
    
    init(contents: Data) {
        self.data = contents
    }
    
    func readBytes(length: Int) -> [UInt8] {
        let databuffer = self.data.subdata(in: offsetInFile..<offsetInFile + length)
        offsetInFile += length
        return [UInt8](databuffer)
    }
    
    func readUInt8() -> UInt8 {
        let databuffer = self.data[offsetInFile]
        offsetInFile += 1
        return UInt8(databuffer)
    }
    
    func readInt8() -> Int8 {
        let databuffer = self.data[offsetInFile]
        offsetInFile += 1
        return Int8(bitPattern: databuffer)
    }
    
    func readInt16LE() -> Int16 {
        let databuffer = self.data.subdata(in: offsetInFile..<offsetInFile+2)
        offsetInFile += 2
        return Int16(bitPattern: UInt16(databuffer[0]) | UInt16(databuffer[1]) << 8)
    }
    
    func readInt32() -> Int32 {
        let databuffer = self.data.subdata(in: offsetInFile..<offsetInFile+4)
        offsetInFile += 4
        return Int32(bitPattern: UInt32(databuffer[0]) << 24 | UInt32(databuffer[1]) << 16 | UInt32(databuffer[2]) << 8 | UInt32(databuffer[3]))
    }
    
    func readInt32LE() -> Int32 {
        let databuffer = self.data.subdata(in: offsetInFile..<offsetInFile+4)
        offsetInFile += 4
        return Int32(bitPattern: UInt32(databuffer[0]) | UInt32(databuffer[1]) << 8 | UInt32(databuffer[2]) << 16 | UInt32(databuffer[3]) << 24)
    }
    
    func seek(set: Int){
        offsetInFile = set
    }
    
    func seek(fromCurrent: Int) {
        offsetInFile += fromCurrent
    }
    
    func count() -> Int {
        return data.count;
    }
}
