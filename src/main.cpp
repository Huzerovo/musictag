#include <iostream>
#include <string>
#include <taglib/fileref.h>

// usage: programe [OPTIONS] <file>
// 
// OPTIONS:
//  --list, -l      List tags.
//  --clear, -C     Clear tags
//  --artist, -a    Set artist.
//  --title, -t     Set title.
//  --comment, -c   Set comment.
//
//  --help, -h      Show this help.
//  --version, -v   Show version.
//

int main() {
    TagLib::FileRef f("example.mp3");
    if (f.isNull())
        return -1;
    if (f.tag()->isEmpty()) {
        printf("File tag is empty.");
    }

    std::cout << f.tag()->artist().toCString(true) <<std::endl;

    std::string new_artist;
    std::cin >> new_artist;

    f.tag()->setArtist(TagLib::String(new_artist,TagLib::String::UTF8));

    f.save();
    std::cout << f.tag()->artist().toCString(true) <<std::endl;
    std::cout << f.tag()->title().toCString(true) <<std::endl;
    
    return 0;
}

