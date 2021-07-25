#include "greenWorldInterface.hpp"

 // ----- Private -----

void GreenWorldInterface::CalcOverlayPosition()
{
    const float PAD = 10.0f;
    const int corner = 1; //Top-Left = 0, Top-Right = 1, Bottom-Left = 2, Bottom-Right = 3
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 work_pos = viewport->WorkPos;
    ImVec2 work_size = viewport->WorkSize;
    _overlayPos.x = (corner & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
    _overlayPos.y = (corner & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
    _overlayPosPivot.x = (corner & 1) ? 1.0f : 0.0f;
    _overlayPosPivot.y = (corner & 2) ? 1.0f : 0.0f;
}

// ----- Public -----

GreenWorldInterface::GreenWorldInterface(Core::Ref<Core::WindowManager> window, Core::Ref<Core::Renderer> renderer)
    : Core::UserInterface(window), _renderer(renderer), _showOverlay(true), _showFpsGraph(false)
{
    //Init implot
    ImPlot::CreateContext();

    //Flags
    _windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove;
    _graphFlags = ImPlotAxisFlags_NoTickLabels;

    //Load custom font
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("../res/fonts/JetBrainsMono-Medium.ttf", 18);

    _framesAccumulated.reserve(5);
}

GreenWorldInterface::~GreenWorldInterface()
{    
    ImPlot::DestroyContext();    
}

void GreenWorldInterface::AddElements()
{
    //Menu bar
    if(ImGui::BeginMainMenuBar())
    {
        if(ImGui::BeginMenu("Settings"))
        {
            ImGui::MenuItem("Show overlay", "", &_showOverlay);
            ImGui::MenuItem("Show fps graph", "", &_showFpsGraph);
            ImGui::EndMenu();  
        }         

        if(ImGui::BeginMenu("Rendering"))
        {
            ImGui::MenuItem("Wireframe-Mode");
            ImGui::EndMenu();    
        }  
        ImGui::EndMainMenuBar();
    }      
    
    //Overlay
    if(_showOverlay)
    {
        //Settings
        CalcOverlayPosition();
        ImGui::SetNextWindowBgAlpha(0.35f);
        ImGui::SetNextWindowPos(_overlayPos, ImGuiCond_Always, _overlayPosPivot);
        
        //FPS-Graph
        if(_showFpsGraph)
        {
            //Rescale plot
            static float history = 5.0f;
            ImPlot::SetNextPlotLimitsX(0, history, ImGuiCond_Always);
            ImPlot::SetNextPlotLimitsY(0,1);        

            //Get rendering data
            static float t = 0;
            t += _window->GetDeltaTime();

            if(t >= history)
            {
                t = 0;
                _framesAccumulated.resize(0);
            }            

            _framesAccumulated.emplace_back(glm::vec2(t, _window->GetFps() * 0.0001));            
        }

        if(ImGui::Begin("Example: Simple overlay", &_showOverlay, _windowFlags))
        {
            ImGui::Text("Application average %.2f ms/frame (%.1f FPS)", _window->GetDeltaTime() * 1000.0f, _window->GetFps());
            ImGui::Separator();

            if(_showFpsGraph)
            {
                if (ImPlot::BeginPlot("", NULL, NULL, ImVec2(-1,100), 0, _graphFlags, _graphFlags)) 
                {
                    ImPlot::PlotLine("", &_framesAccumulated[0].x, &_framesAccumulated[0].y, _framesAccumulated.size(), 0, 2 * sizeof(float));
                    ImPlot::EndPlot();
                }   
                ImGui::Separator();
            }
            
            ImGui::Text("Drawcalls: %d", _renderer->GetDrawcalls());
            ImGui::Text("Vertices: %d", _renderer->GetDrawnVertices());
            ImGui::End();
        }
    }
}