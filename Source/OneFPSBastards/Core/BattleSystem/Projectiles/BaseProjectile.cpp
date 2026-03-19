// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/BattleSystem/Projectiles/BaseProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"
#include "Player/MainCharacter.h"

ABaseProjectile::ABaseProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->InitSphereRadius(12.0f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionComponent->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Block);
	CollisionComponent->SetNotifyRigidBodyCollision(true);
	SetRootComponent(CollisionComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 50000.0f;
	ProjectileMovement->MaxSpeed = 5000.0f;
	ProjectileMovement->ProjectileGravityScale = 1.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
}

void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();

	CollisionComponent->OnComponentHit.AddDynamic(this, &ABaseProjectile::HandleHit);
	SetLifeSpan(LifeSeconds);
}

bool ABaseProjectile::LaunchToTarget(const FVector& StartLocation, const FVector& TargetLocation, float ArcParam)
{
	UWorld* World = GetWorld();

	SetActorLocation(StartLocation);

	FVector LaunchVelocity = FVector::ZeroVector;
	const float ClampedArc = FMath::Clamp(ArcParam, 0.0f, 1.0f);

	const float GravityZ = FMath::Abs(World->GetGravityZ());


	const bool bHasSolution = UGameplayStatics::SuggestProjectileVelocity_CustomArc(
		this,
		LaunchVelocity,
		StartLocation,
		TargetLocation,
		ClampedArc,
		GravityZ
	);

	if (!bHasSolution)
	{
		return false;
	}


	ProjectileMovement->Velocity = LaunchVelocity;
	UE_LOG(
	LogTemp,
	Warning,
	TEXT("Projectile velocity: %s | speed: %.2f"),
	*ProjectileMovement->Velocity.ToString(),
	ProjectileMovement->Velocity.Size()
);

	ProjectileMovement->ProjectileGravityScale = 1.0f;
	ProjectileMovement->Activate(true);
	ProjectileMovement->Velocity = LaunchVelocity.GetSafeNormal();
	ProjectileMovement->InitialSpeed = LaunchVelocity.Size();
	
	return true;
}


void ABaseProjectile::HandleHit_Implementation(
	UPrimitiveComponent* HitComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit
)
{
	if (!OtherActor || OtherActor == this || OtherActor == GetOwner())
	{
		return;
	}

	if (OtherActor->IsA<AMainCharacter>())
	{
		UGameplayStatics::ApplyDamage(
			OtherActor,
			Damage,
			GetInstigatorController(),
			this,
			UDamageType::StaticClass()
		);
	}

	Destroy();
}

