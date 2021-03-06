//------------------------------------------------------------------------------
// Copyright (c) 2011-2012 by European Organization for Nuclear Research (CERN)
// Author: Lukasz Janyst <ljanyst@cern.ch>
//------------------------------------------------------------------------------
// XRootD is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// XRootD is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with XRootD.  If not, see <http://www.gnu.org/licenses/>.
//------------------------------------------------------------------------------

#ifndef __XRD_CL_ENV_HH__
#define __XRD_CL_ENV_HH__

#include <map>
#include <string>
#include <utility>
#include <algorithm>

#include "XrdSys/XrdSysPthread.hh"

namespace XrdCl
{
  //----------------------------------------------------------------------------
  //! A simple key value store intended to hold global configuration.
  //! It is able to import the settings from the shell environment, the
  //! variables imported this way supersede these provided from the C++
  //! code.
  //----------------------------------------------------------------------------
  class Env
  {
    public:
      //------------------------------------------------------------------------
      //! Destructor
      //------------------------------------------------------------------------
      virtual ~Env() {}

      //------------------------------------------------------------------------
      //! Get a string associated to the given key
      //!
      //! @return true if the value was found, false otherwise
      //------------------------------------------------------------------------
      bool GetString( const std::string &key, std::string &value );

      //------------------------------------------------------------------------
      //! Associate a string with the given key
      //!
      //! @return false if there is already a shell-imported setting for this
      //!         key, true otherwise
      //------------------------------------------------------------------------
      bool PutString( const std::string &key, const std::string &value );

      //------------------------------------------------------------------------
      //! Get an int associated to the given key
      //!
      //! @return true if the value was found, false otherwise
      //------------------------------------------------------------------------
      bool GetInt( const std::string &key, int &value );

      //------------------------------------------------------------------------
      //! Associate an int with the given key
      //!
      //! @return false if there is already a shell-imported setting for this
      //!         key, true otherwise
      //------------------------------------------------------------------------
      bool PutInt( const std::string &key, int value );

      //------------------------------------------------------------------------
      //! Import an int from the shell environment. Any imported setting
      //! takes precedence over the one set by other means.
      //!
      //! @return true if the setting exists in the shell, false otherwise
      //------------------------------------------------------------------------
      bool ImportInt( const std::string &key, const std::string &shellKey );

      //------------------------------------------------------------------------
      //! Import a string from the shell environment. Any imported setting
      //! takes precedence over the one set by ther means.
      //!
      //! @return true if the setting exists in the shell, false otherwise
      //------------------------------------------------------------------------
      bool ImportString( const std::string &key, const std::string &shellKey );

      //------------------------------------------------------------------------
      //! Get default integer value for the given key
      //! @param key   : the key
      //! @param value : output parameter, default value corresponding to
      //!                the key
      //! @return      : true if a default integer value for the given key
      //!                exists, false otherwise
      //------------------------------------------------------------------------
      bool GetDefaultIntValue( const std::string &key, int &value );

      //------------------------------------------------------------------------
      //! Get default string value for the given key
      //! @param key   : the key
      //! @param value : output parameter, default value corresponding to
      //!                the key
      //! @return      : true if a default string value for the given key
      //!                exists, false otherwise
      //------------------------------------------------------------------------
      bool GetDefaultStringValue( const std::string &key, std::string &value );

      //------------------------------------------------------------------------
      // Lock the environment for writing
      //------------------------------------------------------------------------
      void WriteLock()
      {
        pLock.WriteLock();
      }

      //------------------------------------------------------------------------
      // Unlock the environment
      //------------------------------------------------------------------------
      void UnLock()
      {
        pLock.UnLock();
      }

      //------------------------------------------------------------------------
      // Re-initialize the lock
      //------------------------------------------------------------------------
      void ReInitializeLock()
      {
        // this is really shaky, but seems to work on linux and fork safety
        // is probably not required anywhere else
        pLock.UnLock();
        pLock.ReInitialize();
      }

      //------------------------------------------------------------------------
      // Re-create the lock in the same memory
      //------------------------------------------------------------------------
      void RecreateLock()
      {
        new( &pLock )XrdSysRWLock();
      }

    private:

      //------------------------------------------------------------------------
      // Unify the key, make sure it is not case sensitive and strip it of
      // the XRD_ prefix if necessary
      //------------------------------------------------------------------------
      inline std::string UnifyKey( std::string key )
      {
        //----------------------------------------------------------------------
        // Make the key lower case
        //----------------------------------------------------------------------
        std::transform( key.begin(), key.end(), key.begin(), ::tolower );

        //----------------------------------------------------------------------
        // Strip the `xrd_` prefix if necessary
        //----------------------------------------------------------------------
        static const char prefix[] = "xrd_";
        if( key.compare( 0, sizeof( prefix ) - 1, prefix ) == 0 )
          key = key.substr( sizeof( prefix ) - 1 );

        return key;
      }

      std::string GetEnv( const std::string &key );
      typedef std::map<std::string, std::pair<std::string, bool> > StringMap;
      typedef std::map<std::string, std::pair<int, bool> >         IntMap;

      XrdSysRWLock pLock;
      StringMap    pStringMap;
      IntMap       pIntMap;
  };
}

#endif // __XRD_CL_ENV_HH__
