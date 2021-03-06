// Copyright (c) 2008-2012 Sundog Software, LLC. All rights reserved worldwide.
//#******************************************************************************
//#*
//#*      Copyright (C) 2015  Compro Computer Services
//#*      http://openig.compro.net
//#*
//#*      Source available at: https://github.com/CCSI-CSSI/MuseOpenIG
//#*
//#*      This software is released under the LGPL.
//#*
//#*   This software is free software; you can redistribute it and/or modify
//#*   it under the terms of the GNU Lesser General Public License as published
//#*   by the Free Software Foundation; either version 2.1 of the License, or
//#*   (at your option) any later version.
//#*
//#*   This software is distributed in the hope that it will be useful,
//#*   but WITHOUT ANY WARRANTY; without even the implied warranty of
//#*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
//#*   the GNU Lesser General Public License for more details.
//#*
//#*   You should have received a copy of the GNU Lesser General Public License
//#*   along with this library; if not, write to the Free Software
//#*   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
//#*
//#*    Please direct any questions or comments to the OpenIG Forums
//#*    Email address: openig@compro.net
//#*
//#*
//#*    Please direct any questions or comments to the OpenIG Forums
//#*    Email address: openig@compro.net
//#*
//#*
//#*****************************************************************************


#pragma once

#include <Core-Base/ImageGenerator.h>

#include <Core-PluginBase/PluginContext.h>

#include <osg/Drawable>
#include <osgViewer/View>

#include <OpenThreads/Mutex>

namespace SilverLining
{
    class Atmosphere;
}

namespace OpenIG {
    namespace Plugins {

        // The update callback is just used to mark our bounds dirty each frame
        struct SilverLiningCloudsUpdateCallback : public osg::Drawable::UpdateCallback
        {
            SilverLiningCloudsUpdateCallback() {}

            virtual void update(osg::NodeVisitor*, osg::Drawable* drawable);
        };

        // We also hook in with a bounding box callback to tell OSG how big our cloud volumes are
        struct SilverLiningCloudsComputeBoundingBoxCallback : public osg::Drawable::ComputeBoundingBoxCallback
        {
            typedef std::vector<osg::Camera*>			Cameras;
            Cameras										cameras;

            SilverLiningCloudsComputeBoundingBoxCallback(const Cameras& cs) : cameras(cs) {}

            virtual osg::BoundingBox computeBound(const osg::Drawable&) const;
        };

        // Define an interface for a class that gives us an enviroment map.
        class EnvMapUpdater
        {
        public:
            EnvMapUpdater() : _envMapID(0) {}

            virtual GLint getEnvironmentMap() const {
                return _envMapID;
            }

            virtual void setEnvironmentMap(GLint id) {
                _envMapID = id;
            }

        protected:
            GLint _envMapID;

        };

        // The CloudsDrawable does the actual drawing of the clouds.
        class CloudsDrawable : public osg::Drawable, public EnvMapUpdater
        {
        public:
            CloudsDrawable();
            CloudsDrawable(osgViewer::CompositeViewer* viewer, OpenIG::Base::ImageGenerator* ig, bool forwardPlusEnabled, bool logZEnabled);

            virtual bool isSameKindAs(const Object* obj) const {
                return dynamic_cast<const CloudsDrawable*>(obj) != NULL;
            }
            virtual Object* cloneType() const {
                return new CloudsDrawable();
            }
            virtual Object* clone(const osg::CopyOp& copyop) const {
                return new CloudsDrawable();
            }

            virtual void drawImplementation(osg::RenderInfo& renderInfo) const;

            void setPluginContext(OpenIG::PluginBase::PluginContext* context)
            {
                _pluginContext = context;
            }

            void setLightingBrightness(bool enable, float onDay, float onNight)
            {
                _lightBrightness_enable = true;// we are not reading this enable;
                _lightBrightness_day = onDay;
                _lightBrightness_night = onNight;
            }

            void setTOD(unsigned int hour)
            {
                _todHour = hour;
            }

            static void setEnvironmentMapDirty(bool dirty);
            static void setForwardPlusRange(float range);
            static void setEnableForwardPlus(bool enable);

        protected:
            void initialize();

            osgViewer::CompositeViewer*			_viewer;
            mutable OpenThreads::Mutex			_mutex;
            OpenIG::Base::ImageGenerator*       _ig;
            static bool							_envMapDirty;
            OpenIG::PluginBase::PluginContext*  _pluginContext;
            bool								_lightBrightness_enable;
            float								_lightBrightness_day;
            float								_lightBrightness_night;
            unsigned int						_todHour;
            bool								_forwardPlusEnabled;
            bool								_logZEnabled;

            static bool							_enableDisableForwardPlusSet;
            static bool							_enableDisableForwardPlus;
        private:
            void setUpShaders(SilverLining::Atmosphere *atmosphere, osg::RenderInfo& renderInfo) const;
            void initializeForwardPlus(SilverLining::Atmosphere *atmosphere, osg::RenderInfo& renderInfo, std::vector<GLint>& vecUserShaders) const;

            void initializeLogZDepthBuffer(osg::RenderInfo& renderInfo, std::vector<GLint>& vecUserShaders) const;
        };
    } // namespace
} // namespace
