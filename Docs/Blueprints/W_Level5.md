# W_Level5

## 1. 蓝图定位

本文档由 Docs/BlueprintExports/W_Level5.json 自动整理生成，用于记录当前蓝图的真实导出结构。

## 2. 基础信息

1. 蓝图名：W_Level5
1. 资源路径：/Script/UMGEditor.WidgetBlueprint'/Game/UI/W_Level5.W_Level5'
1. 蓝图类型：WidgetBlueprint
1. 父类：UMG.UserWidget
1. 生成类：W_Level5_C
1. Tick 预测：OnDemand
1. Tick 原因：Anim

## 3. 当前状态

1. 本文档基于蓝图导出数据自动生成。
1. 事件链、控件树、变量和特殊设置以导出内容为准。
1. 如果某些链路只是历史残留、未接入或未运行验证，需要后续人工补充确认。

## 4. 控件树

1. 根控件：CanvasPanel_43

```text
Image_Background [Image] | parent=CanvasPanel_43 | children=无
VerticalBox_0 [VerticalBox] | parent=CanvasPanel_43 | children=无
Title_Background [Image] | parent=CanvasPanel_43 | children=无
CanvasPanel_43 [CanvasPanel] | parent=ROOT | children=Title_Background, VerticalBox_0, Image_Background, TextBlock_0
TextBlock_0 [TextBlock] | parent=CanvasPanel_43 | children=无
```

## 5. 事件图主链

### 5.1 组件事件

1. B_StartGame -> OnReleased
1. B_StartGame -> OnClicked

### 5.2 主执行链

1. B_StartGame.OnReleased
1. B_StartGame.OnClicked
1. Construct -> PlayAnimation

## 6. 关键变量

### 6.2 自动生成的控件变量

1. tittle -> /Script/CoreUObject.Class'/Script/UMG.WidgetAnimation'

## 7. 特殊设置

1. W_Level5.TickPrediction = OnDemand
   原因：Anim
1. Image_Background.Brush = (ImageType=FullColor,ImageSize=(X=4096.000000,Y=2160.000000),ResourceObject="/Script/Engine.Texture2D'/Game/assets/StartMenu/背景.背景'")
1. Image_Background.Visibility = SelfHitTestInvisible
1. Title_Background.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/StartMenu/tittle.tittle'")
1. Title_Background.Visibility = SelfHitTestInvisible
1. TextBlock_0.Text = NSLOCTEXT("[85ACFA111C6953EB0CD94B858D340D0A]", "64E68D3440F1915A7B8D6F84E798DED4", "我是证书")
1. Title_Background.LayoutData = (Offsets=(Left=43.477478,Top=-412.540527,Right=976.876831,Bottom=604.564575),Anchors=(Minimum=(X=0.500000,Y=0.500000),Maximum=(X=0.500000,Y=0.500000)),Alignment=(X=0.500000,Y=0.000000))
1. VerticalBox_0.LayoutData = (Offsets=(Top=275.459473,Right=848.176697,Bottom=154.972122),Anchors=(Minimum=(X=0.500000,Y=0.500000),Maximum=(X=0.500000,Y=0.500000)),Alignment=(X=0.500000,Y=0.000000))
1. Image_Background.LayoutData = (Offsets=(Right=0.000000,Bottom=0.000000),Anchors=(Maximum=(X=1.000000,Y=1.000000)))
1. TextBlock_0.LayoutData = (Offsets=(Left=481.117126,Top=759.855835,Right=1231.531616,Bottom=275.435455))

## 8. 其他图表

### 8.1 Construct

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：2
1. 连接数：1
1. 执行链：Construct -> ExecuteUbergraph_W_Level5
1. 关键节点：
   - ExecuteUbergraph_W_Level5
     输入：execute <- Construct:then
   - Construct

## 9. 当前可直接确认的结论

1. 本蓝图至少存在上文列出的执行链，可作为后续人工确认和接线补全文档的基础。
1. Widget 树已经从导出中整理出来，后续可以继续补充哪些控件参与了实际逻辑。
1. 变量区已经整理完成，后续重点是确认哪些变量仍然在真实运行链中使用。

## 10. 后续补充建议

1. 如果该蓝图已经过运行验证，建议补充一节“已运行验证链路”。
1. 如果导出中存在历史残留变量、残留事件或旧链路，建议后续在本文档中手动标记。
1. 如果本蓝图和别的蓝图存在强关联，建议在文档中补“对外依赖”。
