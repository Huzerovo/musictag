#include "image.h"
#include <iostream>
#include <taglib/mpegproperties.h>
#include <taglib/tmap.h>
#include <taglib/tstring.h>

bool readImage(const TagLib::FileRef fr, TagLib::ByteVector *data) {
  if (fr.isNull()) {
    std::cerr << "readImage: File doesn't exist." << std::endl;
    return false;
  }

  auto props = fr.complexProperties("PICTURE");
  if (props.isEmpty()) {
    std::cerr << "readImage: File doesn't have a picture tag." << std::endl;
    return false;
  }

  for (auto map : props) {
    if (map.find("data") != map.end()) {
      *data = map["data"].toByteVector();
      return true;
    }
  }

  std::cerr << "readImage: Can not find the picture data." << std::endl;

  return false;
}

bool writeImage(TagLib::FileRef *fr, TagLib::ByteVector data) {
  if (!fr || fr->isNull()) {
    std::cerr << "writeImage: File doesn't exist." << std::endl;
    return false;
  }

  fr->setComplexProperties("PICTURE", {{{"data", data},
                                        {"pictureType", "Front Cover"},
                                        {"mimeType", "image/jpeg"}}});
  return true;
}
