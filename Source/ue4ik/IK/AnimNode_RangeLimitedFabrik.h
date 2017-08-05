// Copyright(c) Henry Cooney 2017

#pragma once

#include "CoreMinimal.h"
#include "IK/IK.h"
#include "BoneControllers/AnimNode_SkeletalControlBase.h"
#include "AnimNode_RangeLimitedFabrik.generated.h"


/*
	Range-limited FABRIK solver. Based on FABRIK UE4 FABRIK solver (see AnimNode_Fabrik.h), but will
	additionally enforce range-of-motion constraints.

	See IK.h for a description of ROM constraints.
*/

USTRUCT()
struct UE4IK_API FAnimNode_RangeLimitedFabrik : public FAnimNode_SkeletalControlBase
{
	GENERATED_USTRUCT_BODY()


	FAnimNode_RangeLimitedFabrik()
		: 
		EffectorTransform(FTransform::Identity),
		EffectorTransformSpace(BCS_ComponentSpace),
		EffectorRotationSource(BRS_KeepLocalSpaceRotation),
		Precision(1.f),
		MaxIterations(10),
		MaxRootDragDistance(0.0f),
		bEnableDebugDraw(false)
	{ }


	// Coordinates for target location of tip bone - if EffectorLocationSpace is bone, this is the offset from Target Bone to use as target location
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EndEffector, meta = (PinShownByDefault))
	FTransform EffectorTransform;

	// Reference frame of Effector Transform. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EndEffector)
	TEnumAsByte<enum EBoneControlSpace> EffectorTransformSpace;

	// If EffectorTransformSpace is a bone, this is the bone to use. *
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EndEffector)
	FBoneReference EffectorTransformBone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bones, meta = (PinShownByDefault))
	URangeLimitedIKChainWrapper* IKChain;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EndEffector)
	TEnumAsByte<enum EBoneRotationSource> EffectorRotationSource;

	// Tolerance for final tip location delta from EffectorLocation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Solver)
	float Precision;

	// Maximum number of iterations allowed, to control performance. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Solver)
	int32 MaxIterations;

	// How far IK may 'drag' the root from its starting position. If set to 0 (or lower), the root remains fixed.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Solver)
	float MaxRootDragDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	bool bEnableDebugDraw;

public:

	// FAnimNode_Base interface
	virtual void GatherDebugData(FNodeDebugData& DebugData) override;
	// End of FAnimNode_Base interface

	// FAnimNode_SkeletalControlBase interface
	virtual void EvaluateSkeletalControl_AnyThread(FComponentSpacePoseContext& Output, TArray<FBoneTransform>& OutBoneTransforms) override;
	virtual bool IsValidToEvaluate(const USkeleton* Skeleton, const FBoneContainer& RequiredBones) override;
	// End of FAnimNode_SkeletalControlBase interface


protected:
	// FAnimNode_SkeletalControlBase interface
	virtual void InitializeBoneReferences(const FBoneContainer& RequiredBones) override;
	// End of FAnimNode_SkeletalControlBase interface

	// Update rotation of parent bone to reflect new position of the child. 
	void UpdateParentRotation(FTransform& ParentTransform, const FIKBone& ParentBone,
		FTransform& ChildTransform, const FIKBone& ChildBone, FCSPose<FCompactPose>& Pose) const;

#if WITH_EDITOR
	// Cached CS location when in editor for debug drawing
	FTransform CachedEffectorCSTransform;
#endif
};
