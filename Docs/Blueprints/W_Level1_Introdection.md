# W_Level1_Introdection

## 1. 蓝图定位

本文档由 Docs/BlueprintExports/W_Level1_Introdection.json 自动整理生成，用于记录当前蓝图的真实导出结构。

## 2. 基础信息

1. 蓝图名：W_Level1_Introdection
1. 资源路径：/Script/UMGEditor.WidgetBlueprint'/Game/UI/W_Level1_Introdection.W_Level1_Introdection'
1. 蓝图类型：WidgetBlueprint
1. 父类：UMG.UserWidget
1. 生成类：W_Level1_Introdection_C

## 3. 当前状态

1. 本文档基于蓝图导出数据自动生成。
1. 事件链、控件树、变量和特殊设置以导出内容为准。
1. 如果某些链路只是历史残留、未接入或未运行验证，需要后续人工补充确认。

## 4. 控件树

1. 根控件：CanvasPanel_16

```text
介绍2 [Image] | parent=CanvasPanel_16 | children=无
介绍1 [Image] | parent=CanvasPanel_16 | children=无
Image_77 [Image] | parent=CanvasPanel_16 | children=无
CanvasPanel_16 [CanvasPanel] | parent=ROOT | children=按钮-关闭, 按钮-上一个, 介绍2, 按钮-下一个, 介绍1, Image_77, BackgroundBlur_64
按钮-关闭 [Button] | parent=CanvasPanel_16 | children=无
按钮-下一个 [Button] | parent=CanvasPanel_16 | children=无
按钮-上一个 [Button] | parent=CanvasPanel_16 | children=无
BackgroundBlur_64 [BackgroundBlur] | parent=CanvasPanel_16 | children=无
```

## 5. 事件图主链

### 5.1 组件事件

1. 按钮-关闭 -> OnClicked
1. 按钮-上一个 -> OnClicked
1. 按钮-下一个 -> OnClicked

### 5.2 主执行链

1. 按钮-关闭.OnClicked -> Cast To CaissonPlayerController -> CaissonPlayerController.SetLevelTargetInteractionEnabled -> CaissonPlayerController.CloseCaissonWidget -> CaissonPlayerController.OpenCaissonWidget
1. 按钮-上一个.OnClicked -> SetVisibility -> SetVisibility
1. 按钮-下一个.OnClicked -> SetVisibility -> SetVisibility
1. Tick
1. Construct -> SetVisibility
1. PreConstruct

## 6. 关键变量

### 6.2 自动生成的控件变量

1. 按钮-关闭 -> /Script/CoreUObject.Class'/Script/UMG.Button'
1. 按钮-下一个 -> /Script/CoreUObject.Class'/Script/UMG.Button'
1. 按钮-上一个 -> /Script/CoreUObject.Class'/Script/UMG.Button'
1. 介绍2 -> /Script/CoreUObject.Class'/Script/UMG.Image'
1. 介绍1 -> /Script/CoreUObject.Class'/Script/UMG.Image'

## 7. 特殊设置

1. 介绍2.Brush = (ImageType=FullColor,ImageSize=(X=4096.000000,Y=1535.000000),ResourceObject="/Script/Engine.Texture2D'/Game/assets/Level1/新手引导/新手引导2.新手引导2'")
1. 介绍2.Visibility = SelfHitTestInvisible
1. 介绍1.Brush = (ImageType=FullColor,ImageSize=(X=4096.000000,Y=1535.000000),ResourceObject="/Script/Engine.Texture2D'/Game/assets/Level1/新手引导/新手引导1.新手引导1'")
1. 介绍1.Visibility = SelfHitTestInvisible
1. Image_77.Brush = (TintColor=(SpecifiedColor=(R=0.158482,G=0.158482,B=0.158482,A=0.500000)))
1. Image_77.Visibility = SelfHitTestInvisible
1. 按钮-关闭.LayoutData = (Offsets=(Left=-101.921875,Top=224.000000,Bottom=100.000000),Anchors=(Minimum=(X=1.000000,Y=0.000000),Maximum=(X=1.000000,Y=0.000000)))
1. 按钮-上一个.LayoutData = (Offsets=(Top=-32.540527,Right=104.881973,Bottom=102.072067),Anchors=(Minimum=(X=0.000000,Y=0.500000),Maximum=(X=0.000000,Y=0.500000)))
1. 介绍2.LayoutData = (Offsets=(Top=-316.540527,Right=0.000000,Bottom=707.248657),Anchors=(Minimum=(X=0.000000,Y=0.500000),Maximum=(X=1.000000,Y=0.500000)))
1. 按钮-下一个.LayoutData = (Offsets=(Left=-101.921875,Top=-56.540527,Right=1.921875,Bottom=102.072067),Anchors=(Minimum=(X=1.000000,Y=0.500000),Maximum=(X=1.000000,Y=0.500000)))
1. 介绍1.LayoutData = (Offsets=(Top=-316.540527,Right=0.000000,Bottom=707.248657),Anchors=(Minimum=(X=0.000000,Y=0.500000),Maximum=(X=1.000000,Y=0.500000)))
1. Image_77.LayoutData = (Offsets=(Left=168.000000,Top=-16.000000,Right=-168.000000,Bottom=16.000000),Anchors=(Maximum=(X=1.000000,Y=1.000000)))
1. BackgroundBlur_64.LayoutData = (Offsets=(Right=0.000000,Bottom=0.000000),Anchors=(Maximum=(X=1.000000,Y=1.000000)))

## 8. 其他图表

### 8.1 BndEvt__W_Level1_Introdection_按钮-上一个_K2Node_ComponentBoundEvent_1_OnButtonClickedEvent__DelegateSignature

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：2
1. 连接数：1
1. 执行链：BndEvt__W_Level1_Introdection_按钮-上一个_K2Node_ComponentBoundEvent_1_OnButtonClickedEvent__DelegateSignature -> ExecuteUbergraph_W_Level1_Introdection
1. 关键节点：
   - ExecuteUbergraph_W_Level1_Introdection
     输入：execute <- K2Node_FunctionEntry_0:6F9CE07F420385AE5BED3BA28A1E4ED4
   - BndEvt__W_Level1_Introdection_按钮-上一个_K2Node_ComponentBoundEvent_1_OnButtonClickedEvent__DelegateSignature

### 8.2 BndEvt__W_Level1_Introdection_按钮-下一个_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：2
1. 连接数：1
1. 执行链：BndEvt__W_Level1_Introdection_按钮-下一个_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature -> ExecuteUbergraph_W_Level1_Introdection
1. 关键节点：
   - ExecuteUbergraph_W_Level1_Introdection
     输入：execute <- K2Node_FunctionEntry_0:723FD4F4425F613B25BC909E137DC331
   - BndEvt__W_Level1_Introdection_按钮-下一个_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature

### 8.3 BndEvt__W_Level1_Introdection_按钮-关闭_K2Node_ComponentBoundEvent_2_OnButtonClickedEvent__DelegateSignature

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：2
1. 连接数：1
1. 执行链：BndEvt__W_Level1_Introdection_按钮-关闭_K2Node_ComponentBoundEvent_2_OnButtonClickedEvent__DelegateSignature -> ExecuteUbergraph_W_Level1_Introdection
1. 关键节点：
   - ExecuteUbergraph_W_Level1_Introdection
     输入：execute <- K2Node_FunctionEntry_0:4633E43E4E941E7E427CBA99C0B67B2F
   - BndEvt__W_Level1_Introdection_按钮-关闭_K2Node_ComponentBoundEvent_2_OnButtonClickedEvent__DelegateSignature

### 8.4 Construct

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：2
1. 连接数：1
1. 执行链：Construct -> ExecuteUbergraph_W_Level1_Introdection
1. 关键节点：
   - ExecuteUbergraph_W_Level1_Introdection
     输入：execute <- K2Node_FunctionEntry_0:954E638F41106BC7899E1A8F1D78A485
   - Construct

## 9. 当前可直接确认的结论

1. 本蓝图至少存在上文列出的执行链，可作为后续人工确认和接线补全文档的基础。
1. Widget 树已经从导出中整理出来，后续可以继续补充哪些控件参与了实际逻辑。
1. 变量区已经整理完成，后续重点是确认哪些变量仍然在真实运行链中使用。

## 10. 后续补充建议

1. 如果该蓝图已经过运行验证，建议补充一节“已运行验证链路”。
1. 如果导出中存在历史残留变量、残留事件或旧链路，建议后续在本文档中手动标记。
1. 如果本蓝图和别的蓝图存在强关联，建议在文档中补“对外依赖”。
