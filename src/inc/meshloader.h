/**
 * @file meshloader.h
 *
 * @author take
 */

#ifndef MESHLOADER_H_
#define MESHLOADER_H_

#include "dataStructure.h"

namespace MeshLoader {

PolygonMesh* readFile(const char* file);

}  // namespace MeshLoader

#endif /* MESHLOADER_H_ */
