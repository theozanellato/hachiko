//
// Created by tzanellato on 11/07/24.
//

#pragma once

#include <JuceHeader.h>
#include <imgui.h>

namespace te = tracktion;

namespace hachiko {

class View {
public:
    View(te::Engine& en, te::Edit& ed)
            : engine (en),
            edit (ed)
    {

    };

    void Draw();
    void DrawTrackControls(te::AudioTrack* track);
    void DrawTimeline();
    void DrawClip(te::Clip * clip);
private:
    te::Engine& engine;
    te::Edit& edit;

};

}// hachiko
