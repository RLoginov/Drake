#ifndef TERRAIN_H_
#define TERRAIN_H_

#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>

class GfxMgr;

class Terrain
{
public:
  Terrain(GfxMgr* gfx);
  virtual ~Terrain();

  virtual void init();


private:
  void defineTerrain(long x, long y);
  void initBlendMaps(Ogre::Terrain* terrain);
  void configureTerrainDefaults(Ogre::Light* light);

  bool mTerrainsImported;
  Ogre::TerrainGroup* mTerrainGroup;
  Ogre::TerrainGlobalOptions* mTerrainGlobals;

  GfxMgr* GFX;

};

#endif /* TERRAIN_H_ */


