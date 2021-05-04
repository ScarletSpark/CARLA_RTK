#include "Carla.h"
#include "Carla/Sensor/RtkSensor.h"
#include "Carla/Game/CarlaEpisode.h"

#include <compiler/disable-ue4-macros.h>
#include "carla/geom/Vector3D.h"
#include <compiler/enable-ue4-macros.h>

#include "DrawDebugHelpers.h"

// #include "Carla/Actor/ActorBlueprintFunctionLibrary.h"
#include "Carla/Util/BoundingBoxCalculator.h"
#include "Carla/Vehicle/CarlaWheeledVehicle.h"

ARtkSensor::ARtkSensor(const FObjectInitializer &ObjectInitializer)
  : Super(ObjectInitializer)
{
  RandomEngine = CreateDefaultSubobject<URandomEngine>(TEXT("RandomEngine"));

  Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereOverlap"));
  Sphere->SetupAttachment(RootComponent);
  // RootComponent = Sphere;
  Sphere->InitSphereRadius(100.0f);
  // Sphere->SetHiddenInGame(true); // Disable for debugging.
  Sphere->SetCollisionProfileName(FName("OverlapAll"));

  PrimaryActorTick.bCanEverTick = true;
}

FActorDefinition ARtkSensor::GetSensorDefinition()
{
  return UActorBlueprintFunctionLibrary::MakeRtkDefinition();
}

void ARtkSensor::Set(const FActorDescription &ActorDescription)
{
  Super::Set(ActorDescription);
  UActorBlueprintFunctionLibrary::SetRtk(ActorDescription, this);
}

// void ARtkSensor::Tick(float DeltaSeconds)
// {

//   DrawDebugSphere(GetWorld(), GetActorLocation(), GetRange(), 20, FColor::Purple, false, -1, 0, 1);

//   // Super::Tick(DeltaSeconds);
  
//   // GNSS tick modification
//   ASensor::Tick(DeltaSeconds); //not sure about it. Probably it will cause the original GNSS tick() method to work befor this modification. Possible solution: ASensor::Tick(DeltaSeconds);
//   carla::geom::Location Location = GetActorLocation();

//   carla::geom::GeoLocation CurrentLocation = CurrentGeoReference.Transform(Location);

//   // Compute the noise for the sensor
//   const float LatError = RandomEngine->GetNormalDistribution(0.0f, LatitudeDeviation);
//   const float LonError = RandomEngine->GetNormalDistribution(0.0f, LongitudeDeviation);
//   const float AltError = RandomEngine->GetNormalDistribution(0.0f, AltitudeDeviation);

//   if(!RtkRoverFlag) //if Base, then saving correction;
//   {
//     SetCorrection(LatError,LonError,AltError);
//   }
//   else // if Rover
//   {
//     TArray<AActor *> DetectedActors;
//     Sphere->GetOverlappingActors(DetectedActors, ARtkSensor::StaticClass()); // collecting all RtkSensors in a radius
//     // DetectedActors.Remove(GetOwner());

//     if (DetectedActors.Num() > 0) // if any RtkSensor in range
//     {
//       float min_distance = FVector::Dist( GetActorLocation(), DetectedActors[0]->GetActorLocation() ); 
//       float distance;
//       int counter = 0;
//       for (int Index = 0; Index != DetectedActors.Num(); ++Index) // looking for the nearest of them
//       {
//         if (DetectedActors[Index] != this)
//         {
//           distance = FVector::Dist( GetActorLocation(), DetectedActors[Index]->GetActorLocation() );
//           if (distance < min_distance)
//           {
//             min_distance = distance;
//             counter = Index;
//           }
//         }
//       }

//       // SetCorrection(DetectedActors[counter]->GetCorrection()); //adding rtk correction as coordinate's bias
//       ARtkSensor *RSensor = Cast<ARtkSensor>(DetectedActors[counter]);
//       if (RSensor)
//       {
//         SetCorrection(RSensor->GetCorrection());
//       }

//     }
//     else //if there is no Sensors around
//     {
//       SetCorrection(0,0,0); // nullifying the correction
//     }
//   }
//   // Apply the noise to the sensor

//   double Latitude, Longitude, Altitude;

//   if (GeoFlag)
//   { 
//     Latitude = CurrentLocation.latitude + LatitudeBias + LatError;
//     Longitude = CurrentLocation.longitude + LongitudeBias + LonError;
//     Altitude = CurrentLocation.altitude + AltitudeBias + AltError;
//   }
//   else
//   {
//     Latitude = Location.x + LatitudeBias + LatError;
//     Longitude = Location.y + LongitudeBias + LonError;
//     Altitude = Location.z + AltitudeBias + AltError;
//   }

//   auto Stream = GetDataStream(*this);
//   Stream.Send(*this, carla::geom::GeoLocation{Latitude-CorrectionX, Longitude-CorrectionY, Altitude-CorrectionZ}); // applying the correction and sending it to the Python API.

// }

void ARtkSensor::Tick(float DeltaSeconds)
{

  DrawDebugSphere(GetWorld(), GetActorLocation(), GetRange(), 20, FColor::Purple, false, -1, 0, 1);

  // Super::Tick(DeltaSeconds);
  
  // GNSS tick modification
  ASensor::Tick(DeltaSeconds); //not sure about it. Probably it will cause the original GNSS tick() method to work befor this modification. Possible solution: ASensor::Tick(DeltaSeconds);
  carla::geom::Location Location = GetActorLocation();

  carla::geom::GeoLocation CurrentLocation = CurrentGeoReference.Transform(Location);

  // Compute the noise for the sensor
  LatError = RandomEngine->GetNormalDistribution(0.0f, LatitudeDeviation);
  LonError = RandomEngine->GetNormalDistribution(0.0f, LongitudeDeviation);
  AltError = RandomEngine->GetNormalDistribution(0.0f, AltitudeDeviation);

  if(!RtkRoverFlag) //if Base, then saving correction;
  {
    SetCorrection(LatError,LonError,AltError);
  }
  else // if Rover
  {
    TArray<AActor *> DetectedActors;
    Sphere->GetOverlappingActors(DetectedActors, ARtkSensor::StaticClass()); // collecting all RtkSensors in a radius
    // DetectedActors.Remove(GetOwner());

    if (DetectedActors.Num() > 0) // if any RtkSensor in range
    {
      float min_distance = FVector::Dist( GetActorLocation(), DetectedActors[0]->GetActorLocation() ); 
      float distance;
      int counter = 0;
      for (int Index = 0; Index != DetectedActors.Num(); ++Index) // looking for the nearest of them
      {
        if (DetectedActors[Index] != this)
        {
          distance = FVector::Dist( GetActorLocation(), DetectedActors[Index]->GetActorLocation() );
          if (distance < min_distance)
          {
            min_distance = distance;
            counter = Index;
          }
        }
      }

      // SetCorrection(DetectedActors[counter]->GetCorrection()); //adding rtk correction as coordinate's bias
      ARtkSensor *RSensor = Cast<ARtkSensor>(DetectedActors[counter]);
      if (RSensor)
      {
        SetCorrection(RSensor->GetCorrection());
      }

    }
    else //if there is no Sensors around
    {
      SetCorrection(0,0,0); // nullifying the correction
    }
  }
  // Apply the noise to the sensor

  double Latitude, Longitude, Altitude;

  if (GeoFlag)
  { 
    Latitude = CurrentLocation.latitude + LatitudeBias + LatError;
    Longitude = CurrentLocation.longitude + LongitudeBias + LonError;
    Altitude = CurrentLocation.altitude + AltitudeBias + AltError;
  }
  else
  {
    Latitude = Location.x + LatitudeBias + LatError;
    Longitude = Location.y + LongitudeBias + LonError;
    Altitude = Location.z + AltitudeBias + AltError;
  }

  auto Stream = GetDataStream(*this);
  Stream.Send(*this, carla::geom::GeoLocation{Latitude-CorrectionX, Longitude-CorrectionY, Altitude-CorrectionZ}); // applying the correction and sending it to the Python API.

}

FVector ARtkSensor::GetCorrection()
{
  return FVector(CorrectionX, CorrectionY, CorrectionZ);
}

void ARtkSensor::SetCorrection(float X, float Y, float Z)
{
  CorrectionX = X;
  CorrectionY = Y;
  CorrectionZ = Z;

  return;
}

void ARtkSensor::SetCorrection(FVector &vec)
{
  CorrectionX = vec.X;
  CorrectionY = vec.Y;
  CorrectionZ = vec.Z;

  return;
}

void ARtkSensor::SetCorrection(FVector vec)
{
  CorrectionX = vec.X;
  CorrectionY = vec.Y;
  CorrectionZ = vec.Z;

  return;
}

void ARtkSensor::SetRange(float Value)
{
  constexpr float M_TO_CM = 100.0f; // Unit conversion.

  Sphere->SetRelativeLocation(FVector{0.0f, 0.0f, 0.0f});

  if(RtkRoverFlag)
    Sphere->SetSphereRadius( 0.1 * M_TO_CM);
  else
    Sphere->SetSphereRadius(Value * M_TO_CM);
}

void ARtkSensor::SetRoverFlag(bool Value)
{
  RtkRoverFlag = Value;
}

float ARtkSensor::GetRange()
{
  return Sphere->GetUnscaledSphereRadius();
}

bool ARtkSensor::GetRoverFlag()
{
  return RtkRoverFlag;
}

void ARtkSensor::SetGeoFlag(bool Value)
{
  GeoFlag = Value;
}

void ARtkSensor::SetLatitudeDeviation(float Value)
{
  LatitudeDeviation = Value;
}

void ARtkSensor::SetLongitudeDeviation(float Value)
{
  LongitudeDeviation = Value;
}

void ARtkSensor::SetAltitudeDeviation(float Value)
{
  AltitudeDeviation = Value;
}

void ARtkSensor::SetLatitudeBias(float Value)
{
  LatitudeBias = Value;
}

void ARtkSensor::SetLongitudeBias(float Value)
{
  LongitudeBias = Value;
}

void ARtkSensor::SetAltitudeBias(float Value)
{
  AltitudeBias = Value;
}

bool  ARtkSensor::GetGeoFlag() const
{
  return GeoFlag;
}
float ARtkSensor::GetLatitudeDeviation() const
{
  return LatitudeDeviation;
}
float ARtkSensor::GetLongitudeDeviation() const
{
  return LongitudeDeviation;
}
float ARtkSensor::GetAltitudeDeviation() const
{
  return AltitudeDeviation;
}

float ARtkSensor::GetLatitudeBias() const
{
  return LatitudeBias;
}
float ARtkSensor::GetLongitudeBias() const
{
  return LongitudeBias;
}
float ARtkSensor::GetAltitudeBias() const
{
  return AltitudeBias;
}

void ARtkSensor::BeginPlay()
{
  Super::BeginPlay();

  const UCarlaEpisode* episode = UCarlaStatics::GetCurrentEpisode(GetWorld());
  CurrentGeoReference = episode->GetGeoReference();
}