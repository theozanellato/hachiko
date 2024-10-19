#include <JuceHeader.h>
#include "MainComponent.h"

namespace te = tracktion;

class MainWindow : public juce::DocumentWindow {
public:
    explicit MainWindow(const juce::String &name)
        : juce::DocumentWindow(name, juce::Colours::black, allButtons) {
        setContentOwned(new MainComponent(engine), true);

        centreWithSize(getWidth(), getHeight());
        setVisible(true);
    }

    void closeButtonPressed() override {
        juce::JUCEApplication::getInstance()->systemRequestedQuit();
    }

private:
    te::Engine engine{ProjectInfo::projectName};
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
};

class MinimalApp : public juce::JUCEApplication {
public:
    const juce::String getApplicationName() override { return JUCE_APPLICATION_NAME_STRING; }
    const juce::String getApplicationVersion() override { return JUCE_APPLICATION_VERSION_STRING; }

    void initialise(const juce::String &commandLine) override {
        juce::ignoreUnused(commandLine);
        win.reset(new MainWindow(getApplicationName()));
    }

    void shutdown() override { win = nullptr; }

private:
    std::unique_ptr<MainWindow> win;
};

START_JUCE_APPLICATION(MinimalApp)
