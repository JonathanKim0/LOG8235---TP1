/* UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class YOURGAME_API UFeedbackComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // This function will be called when the pickup is collected
    UFUNCTION(BlueprintCallable, Category = "Feedback")
    void PlayPickupFeedback(FVector Location);

protected:
    // UPROPERTY allows these to be changed in the Details Panel
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    class USoundBase* PickupSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    class UNiagaraSystem* PickupFX; // Or UParticleSystem* if using legacy
}; */