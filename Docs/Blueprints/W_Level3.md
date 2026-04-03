# W_Level3

## 1. 蓝图定位

本文档由 Docs/BlueprintExports/W_Level3.json 自动整理生成，用于记录当前蓝图的真实导出结构。

## 2. 基础信息

1. 蓝图名：W_Level3
1. 资源路径：/Script/UMGEditor.WidgetBlueprint'/Game/UI/W_Level3.W_Level3'
1. 蓝图类型：WidgetBlueprint
1. 父类：UMG.UserWidget
1. 生成类：W_Level3_C

## 3. 当前状态

1. 本文档基于蓝图导出数据自动生成。
1. 事件链、控件树、变量和特殊设置以导出内容为准。
1. 如果某些链路只是历史残留、未接入或未运行验证，需要后续人工补充确认。

## 4. 控件树

1. 根控件：CanvasPanel_30

```text
Button_44 [Button] | parent=CanvasPanel_30 | children=无
CanvasPanel_30 [CanvasPanel] | parent=ROOT | children=Button_44, 背景, EditableTextBox_85
背景 [Image] | parent=CanvasPanel_30 | children=无
EditableTextBox_85 [EditableTextBox] | parent=CanvasPanel_30 | children=无
```

## 5. 事件图主链

### 5.1 组件事件

1. Button_44 -> OnClicked

### 5.2 主执行链

1. PreConstruct
1. Construct
1. Tick
1. Button_44.OnClicked

## 6. 关键变量

### 6.2 自动生成的控件变量

1. Button_44 -> /Script/CoreUObject.Class'/Script/UMG.Button'

## 7. 特殊设置

1. 背景.Brush = (ImageType=FullColor,ImageSize=(X=4096.000000,Y=2159.000000),ResourceObject="/Script/Engine.Texture2D'/Game/assets/Level1/背景模糊层.背景模糊层'")
1. EditableTextBox_85.Text = NSLOCTEXT("[BB8C6375CC95ABC679EF6811644C7252]", "A7F981064DB207C5B28F73A78A7A06B7", "122333")
1. Button_44.LayoutData = (Offsets=(Left=264.264252,Top=209.352203,Right=300.772186,Bottom=205.032166))
1. 背景.LayoutData = (Offsets=(Right=0.000000,Bottom=0.000000),Anchors=(Maximum=(X=1.000000,Y=1.000000)))
1. EditableTextBox_85.LayoutData = (Offsets=(Left=700.000000,Top=724.000000,Right=604.504517,Bottom=245.072205))

## 8. 其他图表

### 8.1 BndEvt__W_Level3_Button_44_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：2
1. 连接数：1
1. 执行链：BndEvt__W_Level3_Button_44_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature -> ExecuteUbergraph_W_Level3
1. 关键节点：
   - BndEvt__W_Level3_Button_44_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature
   - ExecuteUbergraph_W_Level3
     输入：execute <- K2Node_FunctionEntry_0:4CFDB64F427700A362DF84A5BFE9A0DE

### 8.2 ExecuteUbergraph_W_Level3

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：2
1. 连接数：0
1. 执行链：Button_44.OnClicked
1. 执行链：ExecuteUbergraph_W_Level3
1. 关键节点：
   - Button_44.OnClicked
   - ExecuteUbergraph_W_Level3

## 9. 当前可直接确认的结论

1. 本蓝图至少存在上文列出的执行链，可作为后续人工确认和接线补全文档的基础。
1. Widget 树已经从导出中整理出来，后续可以继续补充哪些控件参与了实际逻辑。
1. 变量区已经整理完成，后续重点是确认哪些变量仍然在真实运行链中使用。

## 10. 后续补充建议

1. 如果该蓝图已经过运行验证，建议补充一节“已运行验证链路”。
1. 如果导出中存在历史残留变量、残留事件或旧链路，建议后续在本文档中手动标记。
1. 如果本蓝图和别的蓝图存在强关联，建议在文档中补“对外依赖”。
