//  StringHelper.h
//  EGE Framework
//
//  Created by enmaniac on 7/13/14.
//  Copyright (c) 2014 Little Bee Studios. All rights reserved.
//

/*! Helper class for converting strings between native Framework and iOS types.
 */

#include "EGEString.h"
#include "EGEText.h"
#import <Foundation/NSString.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class StringHelper
{
  public:
  
    /*! Converts framework's string to native Cocoa string.
     *  @param  string  Framework's string to convert.
     *  @return Returns converted autoreleased string.
     */
    static NSString* Convert(const String& string);
    /*! Converts framework's wide string to native Cocoa string.
     *  @param  string  Framework's wide string to convert.
     *  @return Returns converted autoreleased string.
     */
    static NSString* Convert(const Text& string);
  
    /*! Converts Cocoa string to EGE string representation.
     *  @param  string  Cocoa string to convert.
     *  @return Framework's string representation.
     */
    static String ConvertToString(NSString* string);
    /*! Converts Cocoa string to EGE wide string representation.
     *  @param  string  Cocoa string to convert.
     *  @return Framework's wide string representation.
     */
    static Text ConvertToText(NSString* string);
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

