import UIKit

class BinaryHelper {
    class func load(url: URL, completion: @escaping (_ filename: InMemoryFile) -> ()) {
        let sessionConfig = URLSessionConfiguration.default
        let session = URLSession(configuration: sessionConfig)
        let request = URLRequest(url: url)
        
        let task = session.downloadTask(with: request) { (tempLocalUrl, response, error) in
            if let tempLocalUrl = tempLocalUrl, error == nil {
                if let statusCode = (response as? HTTPURLResponse)?.statusCode {
                    print("Success: \(statusCode)")
                }
                var data : Data
                do {
                    try data = FileHandle(forReadingFrom: tempLocalUrl).readDataToEndOfFile()
                    let file = InMemoryFile(contents: data)
                    file.filename = (response?.suggestedFilename)!
                    completion(file)
                } catch (let writeError) {
                    print("error  \(writeError)")
                }
                
            } else {
                print("Failure: %@", error?.localizedDescription ?? "");
            }
        }
        task.resume()
    }
}
