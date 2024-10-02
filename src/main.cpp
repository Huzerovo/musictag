#include <iostream>
#include <taglib/fileref.h>
#include <unistd.h>

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
void usage()
{
    std::cout << "usage: musictag [OPTIONS] <file>" << std::endl;
    std::cout << std::endl;
    std::cout << "OPTIONS:" << std::endl;
    std::cout << "  --list, -l\t\tList tags." << std::endl;
    std::cout << "  --artist, -a\t\tSet artist." << std::endl;
    std::cout << "  --title, -t\t\tSet title." << std::endl;
    std::cout << "  --comment, -c\t\tSet comment." << std::endl;
    std::cout << std::endl;
    std::cout << "  --help, -h\t\tShow this help." << std::endl;
    std::cout << "  --version, -v\t\tShow version." << std::endl;
}

int main(int argc, char **argv)
{
    bool list_flag = false;
    char *title    = NULL;
    char *artist   = NULL;
    char *comment  = NULL;
    int c;
    while ((c = getopt(argc, argv, "la:t:c:")) != -1) {
        switch (c) {
        case 'l':
            list_flag = true;
            break;
        case 'a':
            artist = optarg;
            break;
        case 't':
            title = optarg;
            break;
        case 'c':
            comment = optarg;
            break;
        case '?':
            if (optopt == 'a' || optopt == 't' || optopt == 'c') {
                fprintf(stderr, "Option -%c requires an argument.\n", optopt);
            } else if (isprint(optopt)) {
                fprintf(stderr, "Unknow option '-%c'.\n", optopt);
            } else {
                fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
            }
            usage();
            return 1;
        default:
            abort();
        }
    }

    if (optind >= argc) {
        fprintf(stderr, "Need a file name.\n");
        usage();
        return 1;
    }

    char *filename = argv[optind];

    TagLib::FileRef f(argv[optind]);

    if (f.isNull()) {
        fprintf(stderr, "Can not open file: %s.\n", filename);
        return 1;
    }
    if (f.tag()->isEmpty()) {
        printf("File tag is empty.");
    }

    if (list_flag) {
        std::cout << "Title: " << f.tag()->title() << std::endl;
        std::cout << "Artist: " << f.tag()->artist() << std::endl;
        std::cout << "Comment: " << f.tag()->comment() << std::endl;
        return 0;
    }

    if (artist) {
        f.tag()->setArtist(TagLib::String(artist, TagLib::String::UTF8));
    }
    if (title) {
        f.tag()->setTitle(TagLib::String(title, TagLib::String::UTF8));
    }
    if (comment) {
        f.tag()->setComment(TagLib::String(comment, TagLib::String::UTF8));
    }

    f.save();

    return 0;
}
