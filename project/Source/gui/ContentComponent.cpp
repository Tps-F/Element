/*
    ContentComponent.cpp - This file is part of Element
    Copyright (C) 2015  Kushview, LLC.  All rights reserved.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "gui/GuiApp.h"
#include "gui/GraphEditorView.h"
#include "gui/ContentComponent.h"
#include "gui/RackView.h"
#include "gui/SequencerComponent.h"
#include "gui/TransportBar.h"
#include "EngineControl.h"

namespace Element {

ContentComponent::ContentComponent (GuiApp& app_)
    : gui (app_)
{
    AudioEnginePtr engine = gui.globals().engine();
    Shared<EngineControl> ctl = engine->controller();
    playbackMonitor = gui.session()->getPlaybackMonitor();
    
    setOpaque (true);
    
    addAndMakeVisible (bar1 = new StretchableLayoutResizerBar (&layoutVertical, 1, false));
    addAndMakeVisible (transport = new Element::TransportBar (gui.session()));
    addAndMakeVisible (graph = new GraphEditorView (gui, *ctl));
    addAndMakeVisible (rack = new RackView());
    
    layoutVertical.setItemLayout (0, 300.0f, -1.0f, 500.0f);
    layoutVertical.setItemLayout (1, 4, 4, 4);
    layoutVertical.setItemLayout (2, 300.0f, 300.0f, 300.0f);
    
    startTimer (17);
    resized();
}

ContentComponent::~ContentComponent()
{
    toolTips = nullptr;
}

void ContentComponent::childBoundsChanged (Component* child)
{
}

void ContentComponent::paint (Graphics &g)
{
    g.fillAll (Colours::darkgrey);
}

void ContentComponent::resized()
{
    const Rectangle<int> r (getLocalBounds().reduced (2));
    transport->setBounds (2, 2, transport->getWidth(), transport->getHeight());

#if 0
    graph->setBounds (r.withTrimmedTop (transport->getHeight() + 2));
#else
    Component* comps[3] = { graph.get(), bar1.get(), rack.get() };
    layoutVertical.layOutComponents (comps, 3,
                                     0,
                                     2 + transport->getHeight(),
                                     getWidth(),
                                     getHeight() - 2 + transport->getHeight(),
                                     true, true);
    
#endif
}

void ContentComponent::setRackViewComponent (Component* comp)
{
    rack->setMainComponent(comp);
}

GuiApp& ContentComponent::app() { return gui; }

void ContentComponent::stabilize()
{
    graph->resized();
    transport->stabilize();
}

void ContentComponent::timerCallback()
{
    transport->setBeatTime (playbackMonitor->get());
}
    
}


