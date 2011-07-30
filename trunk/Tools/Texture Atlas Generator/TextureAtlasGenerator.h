#ifndef TEXTURE_ATLAS_GENERATOR_H
#define TEXTURE_ATLAS_GENERATOR_H

#include <EGEImage.h>
#include <EGEXml.h>
#include <EGEList.h>

#include "AtlasNode.h"
#include "AtlasGroup.h"

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
    /*! Returns input data file path. */
    inline const EGE::String& inputDataFilePath() const { return m_inputDataFilePath; }
    /*! Returns output XML file path. */
    inline const EGE::String& outputXmlPath() const { return m_outputXmlPath; }
    /* Generates atlases. */
    bool generateAll();
    /* Generates atlas for given group. */
    bool generate(AtlasGroup* group);
    /*! Returns current sorting method being used. */
    inline ESortMethod sortMethod() const { return m_sortMethod; }
    
	private:

    /*! Output image format. */
    EGE::EGEImage::Format m_outputFormat;
    /*! Input data file path. */
    EGE::String m_inputDataFilePath;
    /*! Output XML data file path. */
    EGE::String m_outputXmlPath;
    /*! Current sorting method. */
    ESortMethod m_sortMethod;
    /*! XML document with atlas generated data. */
    EGE::PXmlDocument m_atlasData;
    /*! Atlas groups. */
    EGE::List<AtlasGroup*> m_groups;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // TEXTURE_ATLAS_GENERATOR_H