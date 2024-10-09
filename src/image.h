#ifndef _IMAGE_H
#define _IMAGE_H

#include <taglib/fileref.h>
#include <taglib/tbytevector.h>

/*
 * Read image from the file tag .
 * @param fr The file ref pointer
 * @param data The image data
 *
 * @return result of reading
 */
bool readImage(const TagLib::FileRef fr, TagLib::ByteVector *data);

/*
 * Write image to the file tag
 * @param fr The file ref pointer
 * @param data The image data
 *
 * @return result of writing
 */
bool writeImage(TagLib::FileRef *fr, TagLib::ByteVector data);

#endif
