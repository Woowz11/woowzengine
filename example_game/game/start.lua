CreateWindow("Window")
SetWindowMain("Window")
SetWindowImGui("Window")
CreateScene("Scene")
SetWindowScene("Window","Scene")


CreateImGuiWindow("ImGui","Title")

CreateImGuiElement("E1","ImGui","Text")

Print(ToNumber(0))
Print(ToNumber(0.5))
Print(ToNumber(-1))
Print(ToNumber(-2.641))
Print(ToNumber(Infinity))

Print(ToNumber(true))
Print(ToNumber(false))

Print(ToNumber(nil))

Print(ToNumber("string"))
Print(ToNumber("0"))
Print(ToNumber("-52.612"))
Print(ToNumber(Up))
Print(ToNumber(Front))
Print(ToNumber(Red))