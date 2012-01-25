#ifndef FONT_DATA_GENERATOR_H
#define FONT_DATA_GENERATOR_H

#include <EGEImage.h>
#include <EGEXml.h>
#include <EGEList.h>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class FontDataGenerator
{
	 public:

		FontDataGenerator(int argc, char** argv);
	 ~FontDataGenerator();

    /* Returns TRUE if object is valid. */
    bool isValid() const;
    /* Prints syntax. */
    void printSyntax() const;
    /* Processes all data. */
    bool process();

  private:

    /* Prints application info header. */
    void printHeader() const;
    /*! Returns output XML file path. */
    inline const EGE::String& outputXmlPath() const { return m_outputXmlPath; }
    /* Generates atlases. */
    bool generateAll();
    
	private:

    /*! Input data file path. */
    EGE::String m_inputDataFilePath;
    /*! Input texture file path. */
    EGE::String m_inputTextureFilePath;
    /*! Output XML data file path. */
    EGE::String m_outputXmlPath;
    /*! XML document with atlas generated data. */
    EGE::PXmlDocument m_atlasData;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // TEXTURE_ATLAS_GENERATOR_H