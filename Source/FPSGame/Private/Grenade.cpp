// Fill out your copyright notice in the Description page of Project Settings.


#include "Grenade.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FPSProjectile.h"
#include "FPSCharacter.h"
// Sets default values
AGrenade::AGrenade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	SetRootComponent(Mesh);
	Mesh->OnComponentHit.AddDynamic(this, &AGrenade::OnHit);
	Mesh->SetCollisionProfileName("BlockAllDynamic");
	Mesh->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.0f));
	Mesh->CanCharacterStepUpOn = ECB_No;
	Mesh->SetSimulatePhysics(true);
	Force = CreateDefaultSubobject<URadialForceComponent>("RadialForceComponent");
	Force->SetupAttachment(Mesh);
	Force->Radius = 600;
	Force->ForceStrength = 2000;
	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSystemComponent");
	ParticleSystemComponent->SetupAttachment(Mesh);
	ParticleSystemComponent->SetTemplate(ParticleSystem);
}

// Called when the game starts or when spawned
void AGrenade::BeginPlay()
{
	Super::BeginPlay();
	DefaultScale = GetActorScale3D();
}

// Called every frame
void AGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGrenade::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->IsA<AFPSProjectile>())
	{

	}
	else
	{

	FTimerHandle TimerHandle;
	// Starting a timer
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AGrenade::Explode, 2.0f, false);
	}
}

void AGrenade::Explode()
{
	FVector Location = GetActorLocation();
	UGameplayStatics::ApplyRadialDamage(this, 100, Location, Force->Radius, UDamageType::StaticClass(), TArray<AActor*>(), this);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleSystem, Location);
	Destroy();
}

void AGrenade::Pickup(USkeletalMeshComponent* HoldingComp)
{
	Mesh->AttachToComponent(HoldingComp, FAttachmentTransformRules(EAttachmentRule::SnapToTarget,true), "Muzzle");
	Mesh->SetSimulatePhysics(false);

}

void AGrenade::Throw(AFPSCharacter* Player)
{
//	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	Mesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	Mesh->SetSimulatePhysics(true);
	FVector Dir = Player->GetActorForwardVector()*200;
	SetActorScale3D(DefaultScale);
	Mesh->AddImpulse(Dir);
	//Mesh->AddImpulse(Direction * 1200);
	//Mesh->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
}

