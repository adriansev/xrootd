//////////////////////////////////////////////////////////////////////////
//                                                                      //
// XrdClientStringMatcher                                               //
// Author: Fabrizio Furano (INFN Padova, 2004)                          //
//                                                                      //
// Simple class used to match against 'regular expressions formed       //
//  as follows:                                                         //
//   - a sequence of single exprs separated by |                        //
//   - each single expr can have a * char as a wildcard                 //
//     but only in its begin or in its end                              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

//       $Id$

#include <string.h>
#include <stdlib.h>

class XrdClientStringMatcher {
 private:
   // The expr to match
   char *exp;

   bool SingleMatches(char *expr, char *str);

 public:
   XrdClientStringMatcher(char *expr);
   ~XrdClientStringMatcher();

   bool Matches(char *str);

};
