#include <iostream>
#include <memory>

#include <windows.h>

#include <imgui.h>
#include <imgui-SFML.h>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "xbrz/xbrz.h"
#include "hqx/hqx.hpp"
#include "fileOperation/fileOperation.hpp"

inline void endian_swap_ref(uint32_t& x)
{
    x = (x>>24) |
        ((x<<8) & 0x00FF0000) |
        ((x>>8) & 0x0000FF00) |
        (x<<24);
}

inline uint32_t endian_swap(uint32_t x)
{
    return endian_swap_ref(x), x;
}

void nearestNeighborScale(const sf::Image& src, sf::Image& tgr, int32_t scale)
{
    auto imageSize = src.getSize();
    auto newImageSize = imageSize;
    auto pixelCount = imageSize.x * imageSize.y * 4;

    if(scale < -1)
        newImageSize /= (uint32_t)std::abs(scale);
    else if(scale > 1)
        newImageSize *= (uint32_t)scale;

    sf::Uint8* inputBuffer = (sf::Uint8*)malloc(pixelCount);
    memcpy(inputBuffer, src.getPixelsPtr(), pixelCount);

    tgr.create(newImageSize.x, newImageSize.y);

    xbrz::nearestNeighborScale((uint32_t*)inputBuffer, imageSize.x, imageSize.y, (uint32_t*)tgr.getPixelsPtr(), newImageSize.x, newImageSize.y);

    free(inputBuffer);
}

void xbrzScale(const sf::Image& src, sf::Image& tgr, int32_t scale)
{
    int32_t scale2 = scale * scale;

    auto imageSize = src.getSize();
    auto pixelCount = imageSize.x * imageSize.y * 4;

    sf::Uint8* inputBuffer = (sf::Uint8*)malloc(pixelCount);
    memcpy(inputBuffer, src.getPixelsPtr(), pixelCount);

//    for(int x = 0; x < pixelCount; x += 4)
//    {
//        sf::Uint32 p = *(sf::Uint32*)(src.getPixelsPtr() + x);
//        *(sf::Uint32*)(inputBuffer + x) = endian_swap(p);//(p >> 24) | (p << 8);
//    }

    tgr.create(imageSize.x * scale, imageSize.y * scale);

    xbrz::scale(scale, (uint32_t*)inputBuffer, (uint32_t*)tgr.getPixelsPtr(), imageSize.x, imageSize.y, xbrz::ColorFormat::ARGB);

    free(inputBuffer);

//    for(int x = 0; x < pixelCount * scale2; x += 4)
//    {
//        sf::Uint32* p = (sf::Uint32*)(tgr.getPixelsPtr() + x);
////        *p = (*p >> 8) | (*p << 24);
//        *p = endian_swap(*p);
//    }
}

void hqxScale(const sf::Image& src, sf::Image& tgr, int32_t scale)
{
    int32_t scale2 = scale * scale;

    auto imageSize = src.getSize();
    auto pixelCount = imageSize.x * imageSize.y * 4;

    sf::Uint8* inputBuffer = (sf::Uint8*)malloc(pixelCount);
    memcpy(inputBuffer, src.getPixelsPtr(), pixelCount);
    for(uint32_t x = 0; x < pixelCount; x += 4)
    {
        sf::Uint32 p = *(sf::Uint32*)(src.getPixelsPtr() + x);
        *(sf::Uint32*)(inputBuffer + x) = endian_swap(p);
    }

    tgr.create(imageSize.x * scale, imageSize.y * scale);

    if(scale == 2)
        hqx::Scale2((uint32_t*)inputBuffer, (uint32_t*)tgr.getPixelsPtr(), imageSize.x, imageSize.y);
    else if(scale == 3)
        hqx::Scale3((uint32_t*)inputBuffer, (uint32_t*)tgr.getPixelsPtr(), imageSize.x, imageSize.y);
    else if(scale == 4)
        hqx::Scale4((uint32_t*)inputBuffer, (uint32_t*)tgr.getPixelsPtr(), imageSize.x, imageSize.y);

    free(inputBuffer);

    for(uint32_t x = 0; x < pixelCount * scale2; x += 4)
    {
        sf::Uint32* p = (sf::Uint32*)(tgr.getPixelsPtr() + x);
        endian_swap_ref(*p);
    }
}

struct ScalingAlgorithm
{
    const char* name;
    decltype(nearestNeighborScale)* function;

    int32_t minScale;
    int32_t maxScale;
};

struct AlgoSelect
{
    int algoId = 0;
    int scale = 0;
};

char selectFilePath[512] = ".\0";
bool selectFile(const char* name = "Select File", const char* button = "Select", bool requireFileExist = true)
{
    if(ImGui::BeginPopupModal(name, nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        static char buf[sizeof(selectFilePath)] = ".\0";
        bool enter = false;

        std::function<void(std::string)> showFolder = [&](std::string root)
        {
            if(isFolder(root))
            {
                auto folders = getFolderInFolder(root);
                folders.emplace(folders.begin(), "..");
                for(auto& folder : folders)
                {
                    ImGui::SetNextTreeNodeOpen(false);
                    if(ImGui::TreeNodeEx(folder.c_str(), ImGuiTreeNodeFlags_NoTreePushOnOpen))
                    {
//                        showFolder(root + "/" + folder);

                        if(folder == "..")
                        {
                            if(isFolder(root + "/.."))
                            {
                                size_t end = strlen(buf);

                                if((buf[end - 3] == '/' && buf[end - 2] == '.'  && buf[end - 1] == '.') ||
                                (buf[end - 2] == '/' && buf[end - 1] == '.') ||
                                (buf[end - 1] == '.') ||
                                (buf[end - 1] == '/'))
                                    strcpy(buf, (root + "/..").c_str());
                                else
                                {
                                    for(int x = strlen(buf); x >= 0; x--)
                                    {
                                        if(buf[x] == '\\' || buf[x] == '/')
                                        {
                                            buf[x] = '\0';
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                        else
                        {
                            strcpy(buf, (root + "/" + folder).c_str());
                        }

                        strcpy(selectFilePath, buf);
                    }
                }

                for(auto& file : getFilesInFolder(root))
                {
                    ImGui::TreeNodeEx(file.c_str(), ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen);

                    if(ImGui::IsItemClicked())
                    {
                        strcpy(selectFilePath, (root + "/" + file).c_str());

                        enter |= ImGui::IsMouseDoubleClicked(0);
                    }

                    if(ImGui::IsItemHovered())
                    {
                        static sf::Image preview;
                        static sf::Texture previewTexture;

                        if(preview.loadFromFile(root + "/" + file))
                        {
                            auto maxSize = ImGui::GetWindowContentRegionMax();

                            ImGui::BeginTooltip();
                                previewTexture.loadFromImage(preview);

                                sf::Vector2f size = (sf::Vector2f)previewTexture.getSize();

                                if(size.x > maxSize.x)
                                    size *= maxSize.x / size.x;
                                if(size.y > maxSize.y)
                                    size *= maxSize.y / size.y;

                                ImGui::Image(previewTexture, size);
                            ImGui::EndTooltip();
                        }
                    }
                }
            }
        };

        ImGui::BeginChild("File Brownser Child", {0, 300}, true, ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_AlwaysVerticalScrollbar);

        showFolder(buf);

        ImGui::EndChild();

        ImGui::PushItemWidth(450);
        ImGui::InputText("File Path", selectFilePath, sizeof(selectFilePath));

        if(ImGui::IsKeyPressed(ImGui::GetIO().KeyMap[ImGuiKey_Enter]))
        {
            enter |= !ImGui::GetIO().KeyCtrl;

            if(!enter)
                strcpy(buf, selectFilePath);
        }

        if(ImGui::Button("Cancel"))
            ImGui::CloseCurrentPopup();

        if(requireFileExist && !isFile(selectFilePath))
        {
            ImGui::PushStyleColor(ImGuiCol_Button, {0.67f, 0.40f, 0.40f, 0.60f});
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {0.67f, 0.40f, 0.40f, 0.60f});
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, {0.67f, 0.40f, 0.40f, 0.60f});
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5f);
        }

        ImGui::SameLine();
        bool load = (ImGui::Button(button) || enter) && (!requireFileExist || isFile(selectFilePath));

        if(requireFileExist && !isFile(selectFilePath))
        {
            ImGui::PopStyleVar();
            ImGui::PopStyleColor(3);
        }

        if(load)
            ImGui::CloseCurrentPopup();

        ImGui::EndPopup();

        return load;
    }

    return false;
}

int main()
{
    hqx::initialize();

    sf::RenderWindow window(sf::VideoMode(750, 450), "map");

    ImGui::SFML::Init(window);

    sf::Image srcImg;
    srcImg.loadFromFile("img.png");

    sf::Image img;
    sf::Image cmprImg;

    sf::Texture texture;
    sf::Texture cmprTexture;

    ScalingAlgorithm algos[] =
    {
        {"Nearest Neighbor", &nearestNeighborScale, -10, 10},
        {"xBrz", &xbrzScale, 2, 6},
        {"hqx", &hqxScale, 2, 4},
    };
    const int AlgoCount = (int)(sizeof(algos) / sizeof(*algos));

    uint32_t selectionCount = 1;

    std::vector<AlgoSelect> selections;
    selections.assign(selectionCount, {});

    sf::Clock deltaClock;
    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color(255 / 3, 255 / 3, 255 / 3, 255));

        ImGui::SFML::Update(deltaClock.restart());

        ImGui::Begin((std::string(selectFilePath) + "###Data").c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize);

        static bool compare = false;

        texture.loadFromImage(img);

        if(compare)
        {
            cmprTexture.loadFromImage(cmprImg);

            auto curPos = ImGui::GetCursorPosX();
            auto pos = ImGui::GetMousePos().x;

            auto ratio = (ImGui::GetMousePos().x - ImGui::GetCursorScreenPos().x) / (float)texture.getSize().x;

            ratio = std::min(ratio, 1.f);
            ratio = std::max(ratio, 0.f);

            ImGui::Image(texture, sf::FloatRect({0, 0}, {texture.getSize().x * ratio, texture.getSize().y}));
            ImGui::SameLine(0, 0);
            ImGui::Image(cmprTexture, sf::FloatRect({cmprTexture.getSize().x * ratio, 0}, {cmprTexture.getSize().x - cmprTexture.getSize().x * ratio, cmprTexture.getSize().y}));
        }
        else
            ImGui::Image(texture);

        bool saveImg = false;

        static size_t currentSelection = 0;

        currentSelection = std::min(currentSelection, selections.size() - 1);

        if(ImGui::IsKeyPressed(ImGui::GetIO().KeyMap[ImGuiKey_LeftArrow]))
            selections[currentSelection].scale--;
        if(ImGui::IsKeyPressed(ImGui::GetIO().KeyMap[ImGuiKey_RightArrow]))
            selections[currentSelection].scale++;

        if(ImGui::IsKeyPressed(ImGui::GetIO().KeyMap[ImGuiKey_UpArrow]))
            selections[currentSelection].algoId--;
        if(ImGui::IsKeyPressed(ImGui::GetIO().KeyMap[ImGuiKey_DownArrow]))
            selections[currentSelection].algoId++;

        img = srcImg;

        if(compare)
            cmprImg = srcImg;

        static bool comboFocus = false;

        if(ImGui::IsWindowFocused())
            ImGui::SetNextWindowFocus();

        ImGui::Begin("Config", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_NoTitleBar);

        bool noConfig = !ImGui::IsRootWindowOrAnyChildFocused() && !comboFocus;
        comboFocus = false;

        if(noConfig)
            ImGui::End();

        if(!noConfig)
        {
            if(ImGui::Button("Load"))
                ImGui::OpenPopup("Load File");

            if(selectFile("Load File", "Load"))
                srcImg.loadFromFile(selectFilePath);

            ImGui::SameLine();
            if(ImGui::Button("Save"))
                ImGui::OpenPopup("Save File");

            if(selectFile("Save File", "Save", false))
            {
                if(isFile(selectFilePath))
                    ImGui::OpenPopup("Overwrite?");
                else
                    saveImg = true;
            }

            if(ImGui::BeginPopupModal("Overwrite?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text("The file already exist, if you save it will overwrite.\nThis operatio cannot be undone!\n\n");
                ImGui::Separator();

                if (ImGui::Button("Cancel", ImVec2(120,0)))
                {
                    ImGui::CloseCurrentPopup();
                    ImGui::OpenPopup("Save File");
                }

                ImGui::SameLine();
                if (ImGui::Button("Ok", ImVec2(120,0)))
                {
                    ImGui::CloseCurrentPopup();
                    saveImg = true;
                }

                ImGui::EndPopup();
            }

            bool wasCompare = compare;

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {10, 15.f});
            ImGui::SameLine();
            ImGui::Checkbox("Compare", &compare);
            ImGui::PopStyleVar();

            if(!wasCompare && compare)
            {
                auto currentSelections = selections;
                selections.insert(selections.begin() + selectionCount, currentSelections.begin(), currentSelections.end());
            }
            else if(wasCompare && !compare)
                selections.resize(selectionCount);
        }

        const auto& showSelect = [&](uint32_t id)
        {
            if(id == selectionCount)
                ImGui::NextColumn();

            const bool isCompare = id >= selectionCount;

            ImGui::PushID(id);

            AlgoSelect& algoSelect = selections[id];

            auto& algoId = algoSelect.algoId;
            auto& scale = algoSelect.scale;

            if(algoId >= AlgoCount)
                algoId = 0;
            else if(algoId < 0)
                algoId = AlgoCount - 1;

            if(!noConfig)
            {
                if(ImGui::Button("Duplicate"))
                {
                    currentSelection = id;
                    selectionCount += !isCompare;
                    selections.insert(selections.begin() +id, algoSelect);
                }

                if((!isCompare && selectionCount > 1) || (isCompare && (selections.size() - selectionCount) > 1))
                {
                    ImGui::SameLine();
                    if(ImGui::Button("Delete"))
                        selectionCount -= !isCompare, selections.erase(selections.begin() + id);
                }

                if(ImGui::Combo(isCompare ? "Scaling Algorithm  " : "Scaling Algorithm  ", &algoId, [](void* algos, int id, const char** outTxt){*outTxt = ((ScalingAlgorithm*)algos)[id].name; return true;}, &algos, 3))
                    currentSelection = id;

                comboFocus = comboFocus | !ImGui::IsWindowFocused();
            }

            ScalingAlgorithm& currentAlgo = algos[algoId];

            if(!noConfig)
                if(ImGui::SliderInt("Scale", &scale, currentAlgo.minScale, currentAlgo.maxScale))
                    currentSelection = id;

            scale = std::min(scale, currentAlgo.maxScale);
            scale = std::max(scale, currentAlgo.minScale);

            auto& activeImg = isCompare ? cmprImg : img;
            currentAlgo.function(activeImg, activeImg, scale);

            ImGui::PopID();

            if(!noConfig && (id != selectionCount - 1) && (id != selections.size() - 1))
            {
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0, 5.f});
                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.f);
                ImGui::Text("");
                ImGui::PopStyleVar(2);
            }
        };

        if(!noConfig)
        {
            ImGui::Columns(1 + compare);
            ImGui::Separator();
        }

        for(uint32_t x = 0; x < selections.size(); ++x)
            showSelect(x);

        if(!noConfig)
            ImGui::End();

        if(saveImg)
            img.saveToFile(selectFilePath);

        ImGui::End();

        ImGui::Render();
        window.display();
    }

    return 0;
}
