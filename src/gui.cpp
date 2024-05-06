#include "quicknii.hpp"

ImVec2 PLOTDIMENSIONS(1.0f, 1.0f); // Dimensions of the ImPlot plots

static void glfw_error_callback(int error, const char *description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

ImRect CalculateTextureRect(const ImVec2 &textureSize)
{
    // Calculate the scale to fit the texture within the plot without stretching
    float scaleX = PLOTDIMENSIONS.x / textureSize.x;
    float scaleY = PLOTDIMENSIONS.y / textureSize.y;

    // Determine the scale factor based on the cases
    float scale;
    if (textureSize.x > PLOTDIMENSIONS.x || textureSize.y > PLOTDIMENSIONS.y)
    {
        // Case 1 or 2: One or both texture dimensions are larger than the plot
        scale = std::min(scaleX, scaleY);
    }
    else
    {
        // Case 3: Both texture dimensions are smaller than the plot
        scale = std::max(scaleX, scaleY);
    }

    // Calculate the size of the texture after scaling depending on the case
    ImVec2 scaledTextureSize = textureSize * scale;

    // Calculate the position of the texture within the plot
    ImVec2 texturePos = (PLOTDIMENSIONS - scaledTextureSize) * 0.5f;

    // Create the rectangle that represents the texture within the plot
    ImRect rect(texturePos, texturePos + scaledTextureSize);
    return rect;
}

// Main code
int quickniiGUI(int argc, char **argv, internal_nim *nim)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // GL 3.2 + GLSL 150
    const char *glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Required on Mac
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);                      // make this window non-resizable

    // Create window with graphics context
    GLFWwindow *window = glfwCreateWindow(720, 720, "QuickNii", nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.IniFilename = nullptr; 
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    // io.WantCaptureMouse = true;
    // io.WantCaptureKeyboard = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Setup ImPlot context
    ImPlot::CreateContext();

    // Our state
    ImVec4 clear_color = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
    std::vector<GLuint> sliceTexture;

    if (nim->ext == ext_NII)
    {
        sliceTexture = nifti_image_to_slices_gl(nim);
    }
    else if (nim->ext == ext_VMR)
    {
        sliceTexture =  bv_image_to_slices_gl(nim);
    }

    // get the correct coordicnate of the plot
    ImRect sagittalRect = CalculateTextureRect(ImVec2((float)NII_DIM[1], (float)NII_DIM[2]));
    ImRect coronalRect = CalculateTextureRect(ImVec2((float)NII_DIM[0], (float)NII_DIM[2]));
    ImRect axialRect = CalculateTextureRect(ImVec2((float)NII_DIM[0], (float)NII_DIM[1]));

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        static float xs1[1001], ys1[1001];
        for (int i = 0; i < 1001; ++i)
        {
            xs1[i] = i * 0.001f;
            ys1[i] = 0.5f + 0.5f * sinf(50 * (xs1[i] + (float)ImGui::GetTime() / 10));
        }

        static double xs2[20], ys2[20];
        for (int i = 0; i < 20; ++i)
        {
            xs2[i] = i * 1 / 19.0f;
            ys2[i] = xs2[i] * xs2[i];
        }

        const ImGuiViewport *main_viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x, main_viewport->WorkPos.y), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(720, 720), ImGuiCond_FirstUseEver);
        ImGui::Begin("##main", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoDecoration); // No title, no border, no resizes

        // Axis flags for the ImPlots
        static ImPlotAxisFlags axes_flags = ImPlotAxisFlags_Lock |
                                            ImPlotAxisFlags_NoGridLines |
                                            ImPlotAxisFlags_NoTickMarks |
                                            ImPlotAxisFlags_NoDecorations;

        // Create three ImPlots in a 2x2 grid
        for (int i = 0; i < 4; ++i)
        {
            // If this is the fourth iteration (i == 3), create an empty space instead of a plot
            if (i == 3)
            {
                // 3D slider here, now is a dummy
                // ImGui::Dummy(ImVec2(350, 350));
                // static ImVec4 cur3D;
                ImVec4 boundMin(0.0f, 0.0f, 0.0f, 0.0f);
                ImVec4 boundMax((float)NII_DIM[0] - 1, (float)NII_DIM[1] - 1, (float)NII_DIM[2] - 1, 0.0f);
                if (SliderScalar3D("Navigator", &SAG_SLICE_IDX, &COR_SLICE_IDX, &AX_SLICE_IDX,
                                   boundMin.x, boundMax.x,
                                   boundMin.y, boundMax.y,
                                   boundMin.z, boundMax.z,
                                   0.9f))
                {
                    // delete the old texture which is a GLuInt[3]
                    glDeleteTextures(3, &sliceTexture[0]);
                    if (nim->ext == ext_NII)
                    {
                        sliceTexture = nifti_image_to_slices_gl(nim);
                    }
                    else if (nim->ext == ext_VMR)
                    {
                        sliceTexture =  bv_image_to_slices_gl(nim);
                    }
                }
            }
            else
            {
                // Create ImPlots
                if (ImPlot::BeginPlot(("plot" + std::to_string(i)).c_str(), ImVec2(350, 350), ImPlotAxisFlags_NoDecorations))
                {

                    // Stying the plot
                    ImPlot::SetupAxes(NULL, NULL, axes_flags, axes_flags);
                    ImPlot::PushStyleVar(ImPlotStyleVar_PlotPadding, ImVec2(0, 0));

                    // 1 means sagittal, 2 means coronal, 3 means axial -1
                    // Implementation of plot functions here
                    if (i == 0)
                    {
                        ImVec2 uv0 = sagittalRect.Min; // Start of texture coordinates
                        ImVec2 uv1 = sagittalRect.Max; // End of texture coordinates
                        ImPlot::PlotImage("Sagittal", (void *)(intptr_t)sliceTexture[0], uv0, uv1, ImVec2(0, 0), ImVec2(1, 1));
                    }
                    else if (i == 1)
                    {
                        ImVec2 uv0 = coronalRect.Min; // Start of texture coordinates
                        ImVec2 uv1 = coronalRect.Max; // End of texture coordinates
                        ImPlot::PlotImage("Coronal", (void *)(intptr_t)sliceTexture[1], uv0, uv1, ImVec2(0, 0), ImVec2(1, 1));
                    }
                    else if (i == 2)
                    {
                        ImVec2 uv0 = axialRect.Min; // Start of texture coordinates
                        ImVec2 uv1 = axialRect.Max; // End of texture coordinates
                        ImPlot::PlotImage("Axial", (void *)(intptr_t)sliceTexture[2], uv0, uv1, ImVec2(0, 0), ImVec2(1, 1));
                    }

                    ImPlot::EndPlot();
                }
            }
            // If this is not the last plot in the row, create a same-line spacer
            if (i % 2 == 0)
            {
                ImGui::SameLine();
            }
        }

        ImGui::End();

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    glDeleteTextures(3, &sliceTexture[0]);

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}