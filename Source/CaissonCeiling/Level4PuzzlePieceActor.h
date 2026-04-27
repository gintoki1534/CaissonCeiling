#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Level4PuzzlePieceActor.generated.h"

class UStaticMeshComponent;

UCLASS()
class CAISSONCEILING_API ALevel4PuzzlePieceActor : public AActor
{
	GENERATED_BODY()

public:
	ALevel4PuzzlePieceActor();

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Level4")
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Level4")
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(BlueprintReadOnly, Category="Level4")
	int32 PieceIndex = INDEX_NONE;

	UPROPERTY(BlueprintReadOnly, Category="Level4")
	bool bIsSelected = false;

	UPROPERTY(BlueprintReadOnly, Category="Level4")
	bool bIsCorrect = false;

	UPROPERTY(BlueprintReadOnly, Category="Level4")
	bool bIsSpawnedInField = false;

	UFUNCTION(BlueprintCallable, Category="Level4")
	void InitializeLevel4Piece(int32 InPieceIndex);

	UFUNCTION(BlueprintCallable, Category="Level4")
	void SetSelectedVisual(bool bNewSelected);

	UFUNCTION(BlueprintCallable, Category="Level4")
	void SetCorrectVisual(bool bNewCorrect);

	UFUNCTION(BlueprintCallable, Category="Level4")
	UPrimitiveComponent* GetDragPrimitive() const;

	UFUNCTION(BlueprintImplementableEvent, Category="Level4")
	void BP_OnSelectedVisualChanged(bool bNewSelected);

	UFUNCTION(BlueprintImplementableEvent, Category="Level4")
	void BP_OnCorrectVisualChanged(bool bNewCorrect);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level4|Visual")
	bool bUseCustomDepthForSelection = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level4|Visual")
	int32 SelectedCustomDepthStencil = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level4|Visual")
	int32 CorrectCustomDepthStencil = 2;
};
