#include "image.h"

#include <fstream>
#include <getopt.h>
#include <iostream>
#include <taglib/tbytevector.h>
#include <taglib/tpropertymap.h>

#define VERSION "0.0.2"

// Extract cover image from the file, and save it.
void extract_image(TagLib::FileRef fr, const char *extract_to)
{
    TagLib::ByteVector data;
    if (!readImage(fr, &data)) {
        std::cerr << "extract_image: Failed to read image." << std::endl;
        return;
    }
    std::ofstream output(extract_to, std::ios::binary | std::ios::trunc);
    if (!output.is_open()) {
        std::cerr << "extract_image: Can not open file " << extract_to
                  << std::endl;
        return;
    }
    output << data;
    output.close();
    std::cout << "Image has been extract to " << extract_to << std::endl;
}

void import_image(TagLib::FileRef *fr, const char *import_from)
{
    std::ifstream img(import_from);
    if (!img.is_open()) {
        std::cerr << "import_image: Can not open file " << import_from
                  << std::endl;
        return;
    }

    TagLib::ByteVector data;
    img.seekg(0, img.end);
    unsigned int img_size = img.tellg();
    img.seekg(0, img.beg);
    char *buf = new char[img_size + 1];
    img.read(buf, img_size);

    for (int i = 0; i < img_size; i++) {
        data.append(buf[i]);
    }

    img.close();

    if (!writeImage(fr, data)) {
        std::cerr << "import_image: Failed to import image." << std::endl;
    }
}

// Display program version.
void version()
{
    std::cout << "Version: " << VERSION << std::endl;
}

// Display program usage.
void usage()
{
    std::cout << "usage: musictag [OPTIONS] <file>\n\n"
              << "OPTIONS:\n"
              << "  -L, --list              List tags then exit.\n"
              << "      --list-artist       Print the artist.\n"
              << "      --list-title        Print the titler.\n"
              << "      --list-comment      Print the comment.\n"
              << "  -E, --extract <img>     Extract image and save to <img> "
                 "then exit.\n"
              << "  -R, --remove            Remove all tags\n\n"
              << "  -i, --image   <img>     Set image <img> as cover.\n"
              << "  -a, --artist  <string>  Set artist.\n"
              << "  -t, --title   <string>  Set title.\n"
              << "  -c, --comment <string>  Set comment.\n\n"
              << "  -h, --help              Show this help.\n"
              << "  -v, --version           Show version.\n";
}

static int flag_list;
static int flag_list_title;
static int flag_list_artist;
static int flag_list_comment;
static int flag_extract;
static int flag_remove;
static int flag_help;
static int flag_version;

static struct option options_map[] = {
    {         "list",       no_argument,         &flag_list,   1 },
    {   "list-title",       no_argument,   &flag_list_title,   1 },
    {  "list-artist",       no_argument,  &flag_list_artist,   1 },
    { "list-comment",       no_argument, &flag_list_comment,   1 },
    {      "extract", required_argument,      &flag_extract,   1 },
    {       "remove",       no_argument,       &flag_remove,   1 },
    {        "image", required_argument,                  0, 'i' },
    {       "artist", required_argument,                  0, 'a' },
    {        "title", required_argument,                  0, 't' },
    {      "comment", required_argument,                  0, 'c' },
    {         "help",       no_argument,         &flag_help,   1 },
    {      "version",       no_argument,      &flag_version,   1 },
    { 0, 0, 0, 0 }
};

int main(int argc, char **argv)
{
    int c;
    char *title   = NULL;
    char *artist  = NULL;
    char *comment = NULL;

    char *extract_to  = NULL;
    char *import_from = NULL;
    while (1) {
        int option_idx = 0;
        c              = getopt_long(argc, argv, "LE:Ri:a:t:c:hv", options_map,
                                     &option_idx);
        if (c == -1) {
            break;
        }
        switch (c) {
        case 0:
            if (flag_extract) {
                extract_to = optarg;
            }
            break;
        case 'L':
            flag_list = 1;
            break;
        case 'E':
            flag_extract = 1;
            extract_to   = optarg;
            break;
        case 'R':
            flag_remove = 1;
            break;
        case 'i':
            import_from = optarg;
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
        case 'h':
            flag_help = 1;
            break;
        case 'v':
            flag_version = 1;
            break;
        case '?':
            usage();
            return 1;
        default:
            abort();
        }
    }

    if (flag_help) {
        usage();
        return 0;
    }

    if (flag_version) {
        version();
        return 0;
    }

    if (optind >= argc) {
        fprintf(stderr, "Need a file name.\n");
        usage();
        return 1;
    }

    TagLib::FileRef fr(argv[optind]);

    if (fr.isNull()) {
        std::cerr << "Can not open file: " << argv[optind] << std::endl;
        return 1;
    }

    if (flag_list_title) {
        std::cout << fr.tag()->title().toCString(true) << std::endl;
        return 0;
    }

    if (flag_list_artist) {
        std::cout << fr.tag()->artist().toCString(true) << std::endl;
        return 0;
    }

    if (flag_list_comment) {
        std::cout << fr.tag()->comment().toCString(true) << std::endl;
        return 0;
    }

    if (flag_list) {
        std::cout << "Title: " << fr.tag()->title().toCString(true)
                  << std::endl;
        std::cout << "Artist: " << fr.tag()->artist().toCString(true)
                  << std::endl;
        std::cout << "Comment: " << fr.tag()->comment().toCString(true)
                  << std::endl;
        return 0;
    }

    if (flag_extract) {
        if (!extract_to) {
            std::cerr << "main: flag_extract is set but extrace_to is NULL."
                      << std::endl;
            return 1;
        }
        extract_image(fr, extract_to);
        return 0;
    }

    if (flag_remove) {
        fr.removeUnsupportedProperties(fr.properties().unsupportedData());
        TagLib::SimplePropertyMap map;
        fr.setProperties(map);
        fr.save();
        return 0;
    }

    if (artist) {
        fr.tag()->setArtist(TagLib::String(artist, TagLib::String::UTF8));
    }
    if (title) {
        fr.tag()->setTitle(TagLib::String(title, TagLib::String::UTF8));
    }
    if (comment) {
        fr.tag()->setComment(TagLib::String(comment, TagLib::String::UTF8));
    }

    if (import_from) {
        import_image(&fr, import_from);
    }

    fr.save();

    return 0;
}
