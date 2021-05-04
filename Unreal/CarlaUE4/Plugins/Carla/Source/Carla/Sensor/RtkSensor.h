#pragma once

#include "Carla/Sensor/Sensor.h"

#include "Carla/Actor/ActorDefinition.h"
#include "Carla/Actor/ActorDescription.h"

#include "Components/SphereComponent.h"
#include <compiler/disable-ue4-macros.h>
#include "carla/geom/GeoLocation.h"
#include <compiler/enable-ue4-macros.h>

#include "RtkSensor.generated.h"

UCLASS()
class CARLA_API ARtkSensor : public ASensor
{
    GENERATED_BODY()

public:

    ARtkSensor(const FObjectInitializer &ObjectInitializer);

    static FActorDefinition GetSensorDefinition();

    void Set(const FActorDescription &ActorDescription);

    // void SetOwner(AActor *Owner) override; //do not ned to override it, because bounging box isn't depend on parent object

    void Tick(float DeltaSeconds) override;

    void SetGeoFlag(bool Value);
    void SetLatitudeDeviation(float Value);
    void SetLongitudeDeviation(float Value);
    void SetAltitudeDeviation(float Value);

    void SetLatitudeBias(float Value);
    void SetLongitudeBias(float Value);
    void SetAltitudeBias(float Value);

    bool  GetGeoFlag() const;
    float GetLatitudeDeviation() const;
    float GetLongitudeDeviation() const;
    float GetAltitudeDeviation() const;

    float GetLatitudeBias() const;
    float GetLongitudeBias() const;
    float GetAltitudeBias() const;

    // Rtk methods
    void SetRange(float Value);
    void SetRoverFlag(bool Value);
    void SetCorrection(float X, float Y, float Z);
    void SetCorrection(FVector &vec);
    void SetCorrection(FVector vec);
    
    float GetRange();
    bool GetRoverFlag();
    FVector GetCorrection();

protected:

  virtual void BeginPlay() override;

private:

    carla::geom::GeoLocation CurrentGeoReference;

    bool  GeoFlag;

    float LatitudeDeviation;
    float LongitudeDeviation;
    float AltitudeDeviation;

    float LatitudeBias;
    float LongitudeBias;
    float AltitudeBias;

    UPROPERTY(VisibleAnywhere)
    class USphereComponent *Sphere = nullptr;

    // float RtkRadius;
    bool RtkRoverFlag;

    UPROPERTY(VisibleAnywhere)
    float DelayedNoise[3];
    UPROPERTY(VisibleAnywhere)
    float CorrectionX;
    UPROPERTY(VisibleAnywhere)
    float CorrectionY;
    UPROPERTY(VisibleAnywhere)
    float CorrectionZ;

    UPROPERTY(VisibleAnywhere)
    float LatError; 
    UPROPERTY(VisibleAnywhere)
    float LonError;
    UPROPERTY(VisibleAnywhere)
    float AltError;

};