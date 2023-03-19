// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

}

void AGun::PullTrigger() 
{
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
	//attach particle effects [MuzzleFlash] option to Gun [Mesh] at location MuzzleFlashSocket
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleFlashSocket"));
	
	FHitResult Hit;
	FVector ShotDirection;

	bool bSuccess = GunTrace(Hit, ShotDirection);
	if (bSuccess)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location, ShotDirection.Rotation()); //make bullet impact efffect at given location in the level
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, Hit.Location);

		AActor* HitActor = Hit.GetActor(); //the actor that got hit by bullet
		if (HitActor != nullptr)
		{
			FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr); //nature of damage
			AController *OwnerController = GetOwnerController();
			HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this); //actor processes damage
		}
	}

}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AGun::GunTrace(FHitResult &Hit, FVector& ShotDirection) 
{
	AController *OwnerController = GetOwnerController();
	if (OwnerController == nullptr) 
		return false;

	FVector Location;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(Location, Rotation); //gets info on player viewpoint
	ShotDirection = -Rotation.Vector();
	//Shotdirection = where the shot was coming from; the opposite direction of shooter camera

	FVector End = Location + Rotation.Vector() * MaxRange; // end of line trace
	FCollisionQueryParams Params;
	//actors to be ignored as part of the line trace
	Params.AddIgnoredActor(this); //Gun
	Params.AddIgnoredActor(GetOwner()); //shooter holding gun
	//the owner of the gun should not be hit by his own gun

	return GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel1, Params);
	//did the laser hit something in the world
	//FHitResult Hit; //point hit by laser
}

AController* AGun::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner()); //represents shooter character
	if (OwnerPawn == nullptr) 
		return nullptr;
	return OwnerPawn->GetController(); //represents controller linked to shooter character
}

