/*Virtual A/B merge status information*/

// Spaces used by misc partition are as below:
// 0   - 2K     For bootloader_message
// 2K  - 16K    Used by Vendor's bootloader (the 2K - 4K range may be optionally used
//              as bootloader_message_ab struct)
// 16K - 32K    Used by uncrypt and recovery to store wipe_package for A/B devices
// 32K - 64K    System space, used for miscellanious AOSP features. See below.
// Note that these offsets are admitted by bootloader,recovery and uncrypt, so they
// are not configurable without changing all of them.
#define BOOTLOADER_MESSAGE_OFFSET_IN_MISC 0
#define VENDOR_SPACE_OFFSET_IN_MISC 2 * 1024
#define WIPE_PACKAGE_OFFSET_IN_MISC 16 * 1024
#define SYSTEM_SPACE_OFFSET_IN_MISC 32 * 1024
#define SYSTEM_SPACE_SIZE_IN_MISC 32 * 1024

enum MergeStatus {
    /**
     * No snapshot or merge is in progress.
     */
    NONE = 0,

    /**
     * The merge status could not be determined.
     */
    UNKNOWN,

    /**
     * Partitions are being snapshotted, but no merge has been started.
     */
    SNAPSHOTTED,

    /**
     * At least one partition has merge is in progress.
     */
    MERGING,

    /**
     * A merge was in progress, but it was canceled by the bootloader.
     */
    CANCELLED,
};

// Holds Virtual A/B merge status information. Current version is 1. New fields
// must be added to the end.
struct misc_virtual_ab_message {
    uint8_t version;
    uint32_t magic;
    uint8_t merge_status;  // IBootControl 1.1, MergeStatus enum.
    uint8_t source_slot;   // Slot number when merge_status was written.
    uint8_t reserved[57];
};
