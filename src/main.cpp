#include <getopt.h>
#include <iostream>
#include <ostream>
#include <taglib/fileref.h>

#define VERSION "0.0.1"

void version() { std::cout << "Version: " << VERSION << std::endl; }
// usage: musictag [OPTIONS] <file>
//
// OPTIONS:
//  --list, -l      List tags.
//  --artist, -a    Set artist.
//  --title, -t     Set title.
//  --comment, -c   Set comment.
//
//  --help, -h      Show this help.
//  --version, -v   Show version.
//
void usage() {
  std::cout << "usage: musictag [OPTIONS] <file>\n\n"
            << "OPTIONS:\n"
            << "  --list, -l      List tags.\n"
            << "  --artist, -a    Set artist.\n"
            << "  --title, -t     Set title.\n"
            << "  --comment, -c   Set comment.\n\n"
            << "  --help, -h      Show this help.\n"
            << "  --version, -v   Show version.\n";
}

static int flag_list;
static int flag_help;
static int flag_version;
static struct option options_map[] = {
    {"list", no_argument, &flag_list, 1},
    {"artist", required_argument, 0, 'a'},
    {"title", required_argument, 0, 't'},
    {"comment", required_argument, 0, 'c'},
    {"help", no_argument, &flag_help, 1},
    {"version", no_argument, &flag_version, 1}};

int main(int argc, char **argv) {
  int c;
  char *title = NULL;
  char *artist = NULL;
  char *comment = NULL;
  while (1) {
    int option_idx = 0;
    c = getopt_long(argc, argv, "a:t:c:lhv", options_map, &option_idx);
    if (c == -1)
      break;
    switch (c) {
    case 0:
      if (options_map[option_idx].flag)
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
    case 'l':
      flag_list = 1;
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

  TagLib::FileRef f(argv[optind]);

  if (f.isNull()) {
    fprintf(stderr, "Can not open file: %s.\n", argv[optind]);
    return 1;
  }

  if (flag_list) {
    std::cout << "Title: " << f.tag()->title().toCString(true) << std::endl;
    std::cout << "Artist: " << f.tag()->artist().toCString(true) << std::endl;
    std::cout << "Comment: " << f.tag()->comment().toCString(true) << std::endl;
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
