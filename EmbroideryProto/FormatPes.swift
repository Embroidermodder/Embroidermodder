class FormatPes : FormatProtocol {    
    func read(file: InMemoryFile) -> EmbPattern {
        var pecstart : Int
        var numColors : UInt8
        let pattern = EmbPattern()
        file.seek(set: 8)
        pecstart = Int(file.readInt32LE())
        file.seek(set: pecstart + 48)
        numColors = file.readUInt8() + 1
        for _ in 0...numColors {
            pattern.addThread(FormatPec.getPecThreads()[Int(file.readUInt8())])
        }
        file.seek(set: pecstart + 532)
        FormatPec.readPecStitches(pattern: pattern, file : file)
        return pattern
    }
}
