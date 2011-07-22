#ifndef TEXTURE_ATLAS_GENERATOR_H
#define TEXTURE_ATLAS_GENERATOR_H

#include <EGEImage.h>
#include <EGEMap.h>
#include <EGEDynamicArray.h>
#include <EGEXml.h>
#include "ImageEntry.h"
#include "AtlasNode.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class TextureAtlasGenerator
{
	 public:

		TextureAtlasGenerator(int argc, char** argv);
	 ~TextureAtlasGenerator();

    /* Returns TRUE if object is valid. */
    bool isValid() const;
    /* Prints syntax. */
    void printSyntax() const;
    /* Processes all data. */
    bool process();

  private:

    enum ESortMethod
    {
      SM_NONE,
      SM_GREATER_AREA,
      SM_GREATER_WIDTH,
      SM_GREATER_HEIGHT
    };

  private:

    /* Prints application info header. */
    void printHeader() const;
    /*! Returns output image format. */
    inline EGE::EGEImage::Format outputFormat() const { return m_outputFormat; }
    /*! Returns output image size (in pixels). */
    inline EGE::s32 outputSize() const { return m_outputSize; }
    /*! Returns input data file path. */
    inline const EGE::String& inputDataFilePath() const { return m_inputDataFilePath; }
    /* Generates atlases. */
    bool generateAll();
    /* Generates atlas for given group. */
    bool generate(const EGE::String& group);
    /*! Returns current sorting method being used. */
    inline ESortMethod sortMethod() const { return m_sortMethod; }
    /* Returns sorted array of image array pointers belonging to given group. */
    EGE::DynamicArray<ImageEntry*> sortedEntries(const EGE::String& group) const;

	private:

    /*! Output image format. */
    EGE::EGEImage::Format m_outputFormat;
    /*! Output texture size (in pixels). Same for width and height. */
    EGE::s32 m_outputSize;
    /*! Input data file path. */
    EGE::String m_inputDataFilePath;
    /*! Map of all image entries sorted by group Id. */
    EGE::MultiMap<EGE::String, ImageEntry*> m_imageEntries;
    /*! Current sorting method. */
    ESortMethod m_sortMethod;
    /*! XML document with atlas generated data. */
    EGE::PXmlDocument m_atlasData;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // TEXTURE_ATLAS_GENERATOR_H