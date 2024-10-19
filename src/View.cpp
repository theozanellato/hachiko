//
// Created by tzanellato on 11/07/24.
//
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS 1
#endif // IMGUI_DEFINE_MATH_OPERATORS

#include "View.h"
#include <imgui-knobs.h>
#include <ImGuiFileDialog.h>
#include <HachikoImGui.h>
#include <Mouse.h>

namespace hachiko {

const int arrangementTrackHeight = 100;

void View::DrawTrackControls(te::AudioTrack* track) {
    float gain = track->getVolumePlugin()->getVolumeDb();
    float pan = track->getVolumePlugin()->getPan();
    const char * name = track->getName().toRawUTF8();
    ImGui::PushID(name);
    ImGui::BeginGroup();
    ImGui::Text(name);
    if (ImGui::Button("FX")) {}
    ImGui::EndGroup();
    ImGui::SameLine();
    if (ImGuiKnobs::Knob("Volume", &gain, -100.0f, 6.0f, 0.1f, "%.1fdB", ImGuiKnobVariant_WiperDot, 40.0f)) {
        MessageManager::callAsync([track, gain]{track->getVolumePlugin()->setVolumeDb(gain);});
    }
    ImGui::SameLine();
    if (ImGuiKnobs::Knob("Pan", &pan, -1.0f, 1.0f, 0.01f, "%.2f", ImGuiKnobVariant_WiperDot, 40.0f)) {
        MessageManager::callAsync([track, pan]{track->getVolumePlugin()->setPan(pan);});
    }
    ImGui::SameLine();
    if (ImGui::Button("M")) {}
    ImGui::SameLine();
    if (ImGui::Button("S")) {}
    ImGui::PopID();
}

void View::DrawClip(te::Clip * clip) {
    ImGui::PushID(clip);
    ImGui::Button("Clip");
    ImGui::PopID();
}

void View::DrawTimeline() {
    juce::Array<te::AudioTrack *> tracks = te::getAudioTracks(edit);
    int num_tracks = tracks.size();
    auto wpos = ImGui::GetCursorScreenPos();
    const float w = ImGui::GetContentRegionAvail().x;
    const float h = ImGui::GetContentRegionAvail().y;
    te::TimeDuration l = edit.getLength();
    auto draw = ImGui::GetWindowDrawList();
    const float ty = ImGui::GetTextLineHeight();
    const float dx = w / 32.0f;
    float x = 0;
    int bar = 1;
    float playBar = edit.getTransport().getPosition().inSeconds();
    float tempo = edit.tempoSequence.getBpmAt(te::TimePosition{});
    int timeSigNum = edit.tempoSequence.getTimeSigAt(te::TimePosition{}).numerator;
    int timeSigDenom = edit.tempoSequence.getTimeSigAt(te::TimePosition{}).denominator;
    float playCursor = playBar * tempo / (60.0f * timeSigDenom);

    while (x < w) {
        char buf[8];
        sprintf(buf, "%d", bar);
        DrawLine( draw, wpos + ImVec2( x, 0 ), wpos + ImVec2( x, ty ), 0x66FFFFFF );
        draw->AddText( wpos + ImVec2( x+2, 0 ), 0x66FFFFFF, buf );
        x += dx;
        bar++;
    }

    DrawLine( draw, wpos + ImVec2( playCursor * dx, 0 ), wpos + ImVec2( playCursor * dx, h ), 0xFFFFFFFF );

    ImGui::Dummy(ImVec2(0, ImGui::GetTextLineHeight()));
    ImGui::Separator();
    for ( int i = 0; i<num_tracks; i++) {
        ImGui::PushID(i);
        // Draw Clips
        auto clips = tracks[i]->getClips();
        int num_clips = clips.size();
        for (int clip = 0; clip < num_clips; clip++) {
            DrawClip(clips[clip]);
        }
        // Draw Track Background
        ImGui::InvisibleButton("track", ImVec2(w, arrangementTrackHeight));

        if (ImGui::BeginPopupContextItem(nullptr, ImGuiPopupFlags_MouseButtonRight)) {
            if (ImGui::Button("Add Clip...")) {
                const tracktion::TimeRange editTimeRange (te::TimePosition{}, edit.tempoSequence.toTime ({ 1, tracktion::BeatDuration() }));

                MessageManager::callAsync([tracks, i, editTimeRange]{tracks[i]->insertNewClip(te::TrackItem::Type::wave, "Step Clip", editTimeRange, nullptr);});
/*
                // Set the clip's source to be empty
                if (auto waveClip = dynamic_cast<te::WaveAudioClip*>(clip))
                {
                    waveClip->setUsesInternalSource();
                }
*/
            }
            ImGui::EndPopup();
        }
        ImGui::Separator();
        ImGui::PopID();
    }
}


void View::Draw() {
    juce::Array<te::AudioTrack *> tracks = te::getAudioTracks(edit);
    int num_tracks = tracks.size();

    //ImGui::ShowDemoWindow();
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(1500, 800));

    ImGui::Begin("Arrangement", nullptr, ImGuiWindowFlags_NoDecoration);
    {
        // Top
        {
            ImGui::BeginChild("transport", ImVec2(0,-150), ImGuiChildFlags_Border | ImGuiChildFlags_ResizeY);
            if (ImGui::Button("Play")) {
                MessageManager::callAsync([this]{
                    edit.getTransport().play(false);});
            }
            ImGui::SameLine();
            if (ImGui::Button("Pause")) {
                MessageManager::callAsync([this]{
                    edit.getTransport().stop(false, false);});}
            ImGui::SameLine();
            if (ImGui::Button("Stop")) {
                MessageManager::callAsync([this]{
                    edit.getTransport().stop(false, false);
                    edit.getTransport().setPosition (te::TimePosition{});});}
            ImGui::SameLine();
            ImGui::Text("Tempo: %.1f BPM", edit.tempoSequence.getBpmAt(te::TimePosition{}));
            ImGui::EndChild();
        }

        // Left
        {
            ImGui::BeginChild("left pane", ImVec2(150, 0), ImGuiChildFlags_Border | ImGuiChildFlags_ResizeX);
            ImGui::Dummy(ImVec2(0, ImGui::GetTextLineHeight()));
            ImGui::Separator();
            for (int i = 0; i < num_tracks; i++)
            {
                auto wpos = ImGui::GetCursorScreenPos();
                DrawTrackControls(tracks[i]);
                ImGui::SetCursorScreenPos(wpos);
                ImGui::Dummy(ImVec2(0, arrangementTrackHeight));
                ImGui::Separator();
            }
            if (ImGui::Button("Add Track")) {
                MessageManager::callAsync([this, num_tracks]{
                    edit.ensureNumberOfAudioTracks(num_tracks+1);
                });
            }
            ImGui::EndChild();
        }
        ImGui::SameLine();

        // Right
        {
            ImGui::BeginChild("timeline", ImVec2(0, 0), ImGuiChildFlags_Border); // Leave room for 1 line below us
            DrawTimeline();
            ImGui::EndChild();
        }
    }
    ImGui::End();
}

} // hachiko
