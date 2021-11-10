# Ham Game Engine

Powered by [Magnum](https://magnum.graphics/) (OpenGl and C++)

Work in progress...

---

## Dependencies
* [vcpkg](https://vcpkg.io/en/index.html)
* [vscode](https://code.visualstudio.com/) (not required, but makes setting up a lot easier)
    * vscode [C/C++ Estension Pack](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools-extension-pack)
* cmake

---

## Usage

```shell
clone https://github.com/hamyyy/ham-game-engine
cd ./ham-game-engine
git submodule update --init --recursive

vcpkg install

code .
```
Run from vscode using cmake tools

---

#### Submodules

* [Corrade](https://github.com/mosra/corrade)
* [GLFW](https://github.com/glfw/glfw)
* [ImGui](https://github.com/ocornut/imgui) (docking branch)
* [ImGuizmo](https://github.com/CedricGuillemet/ImGuizmo)
* [Magnum](https://github.com/mosra/magnum)
* [Magnum Integration](https://github.com/mosra/magnum-integration)

