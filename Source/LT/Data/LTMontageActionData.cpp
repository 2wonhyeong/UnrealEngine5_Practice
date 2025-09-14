// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/LTMontageActionData.h"

UAnimMontage* ULTMontageActionData::GetMontageForTag(const FGameplayTag& GroupTag, const int32 Index) const
{
	if (MontageGroupMap.Contains(GroupTag))
	{
		const FLTMontageGroup& MontageGroup = MontageGroupMap[GroupTag];

		if (MontageGroup.Animations.Num() > 0 && MontageGroup.Animations.Num() > Index)
		{
			return MontageGroup.Animations[Index];
		}
	}

	return nullptr;
}

UAnimMontage* ULTMontageActionData::GetRandomMontageForTag(const FGameplayTag& GroupTag) const
{
	if (MontageGroupMap.Contains(GroupTag))
	{
		const FLTMontageGroup& MontageGroup = MontageGroupMap[GroupTag];

		const int32 RandomIndex = FMath::RandRange(0, MontageGroup.Animations.Num() - 1);

		return GetMontageForTag(GroupTag, RandomIndex);
	}
	return nullptr;
}
