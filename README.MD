请运行 bin\Release-windows-x86_64\Editor\Editor.exe
或者 bin\Debug-windows-x86_64\Editor\Editor.exe

第一次运行IMGUI的窗口可能都比较小，请手动拉大。
一共有四个窗口：Viewport, debug panel, control panel, Properties
viewport是主要视图窗口。debug panel没啥用，目前只能看游戏运行速度。control panel控制着当前场景，可以通过点击其上面的“Create Entity”按钮创建游戏物体，右键点击列表中的游戏物体可以将其删除，左键点击列表中的游戏物体可以将其选中，左键单击空白处可以取消选中。Properties控制着被选中的物体，可以添加·删除和编辑组件。
此外，option菜单中可以储存和读取储存在asset\scene\文件夹中的场景。默认的autosave文件储存了一个由一个摄像机，一个3D物体，和一个点光源组成的场景。

注：
在你打开exe后，可能会注意到WASD键可以使摄像机移动，这是因为程序开始时给摄像机赋予了一个nativeScriptComponent，而nativeScriptComponent指向了一个类，那个类实现了WASD移动。而类无法写入txt文件，因此在载入场景后，摄像机失去WASD移动功能。
灯光现在只能影响到3D物体，即mesh。2D的sprite不会受光的影响，即使关掉环境光，2Dsprite依旧照常显示。
有很多代码是为了实现跨平台写的，主要负责渲染的代码在Tengine\src\renderers.cpp，负责控制场景的代码在Tengine\src\scenes.cpp和Tengine\include\scene\，负责事件管理的代码在Tengine\include\event\。
本程序使用的三方库：GLAD，GLM，IMGUI，YAML，ENTT，GLFW。