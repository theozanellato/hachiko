/* MainComponent.h: GUI component of ImGui */
#pragma once

#include <JuceHeader.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_juce/imgui_impl_juce.h>

#include <View.h>
#include <Mouse.h>

namespace te = tracktion;

class MainComponent
        : public juce::Component
        , public juce::OpenGLRenderer {
public:
    explicit MainComponent(te::Engine& e)
        : engine (e)
	{
        setOpaque(true);

        // set up opengl context
        glctx.setOpenGLVersionRequired(juce::OpenGLContext::openGL3_2);
        glctx.setRenderer(this);
        glctx.attachTo(*this);
        glctx.setContinuousRepainting(true);

        setSize(1500, 800);
        setWantsKeyboardFocus(true);
    }

    void newOpenGLContextCreated() override {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGui_ImplJuce_Init(*this, glctx);
        ImGui_ImplOpenGL3_Init();
    }

    void renderOpenGL() override {
        using namespace juce::gl;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplJuce_NewFrame();
        ImGui::NewFrame();
        hachiko::MouseFrame();

        // imgui begin
		view.Draw();
        // imgui end

        ImGui::Render();

        // background begin
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        // background end

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void openGLContextClosing() override {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplJuce_Shutdown();
        ImGui::DestroyContext();
    }

    // regular ui not used
    void paint(juce::Graphics &) override {
    }

    void resized() override {
    }

private:
    juce::OpenGLContext glctx;

	te::Engine& engine;
    te::Edit edit { engine, te::createEmptyEdit (engine), te::Edit::forEditing, nullptr, 0 };
    te::TransportControl& transport { edit.getTransport() };

	hachiko::View view { engine, edit };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
