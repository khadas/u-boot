/*
 * Copyright (C) 2012 Avionic Design GmbH
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sub license,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include <common.h>
#include <amlogic/media/vout/hdmitx21/hdmitx.h>
#include "hdmitx_drv.h"

static u8 hdmi_infoframe_checksum(const u8 *ptr, size_t size)
{
	u8 csum = 0;
	size_t i;

	/* compute checksum */
	for (i = 0; i < size; i++)
		csum += ptr[i];

	return 256 - csum;
}

static void hdmi_infoframe_set_checksum(void *buffer, size_t size)
{
	u8 *ptr = buffer;

	ptr[3] = hdmi_infoframe_checksum(buffer, size);
}

/**
 * hdmi_avi_infoframe_init() - initialize an HDMI AVI infoframe
 * @frame: HDMI AVI infoframe
 *
 * Returns 0 on success or a negative error code on failure.
 */
int hdmi_avi_infoframe_init(struct hdmi_avi_infoframe *frame)
{
	memset(frame, 0, sizeof(*frame));

	frame->type = HDMI_INFOFRAME_TYPE_AVI;
	frame->version = 2;
	frame->length = HDMI_AVI_INFOFRAME_SIZE;

	return 0;
}

static int hdmi_avi_infoframe_check_only(const struct hdmi_avi_infoframe *frame)
{
	if (frame->type != HDMI_INFOFRAME_TYPE_AVI ||
	    frame->version != 2 ||
	    frame->length != HDMI_AVI_INFOFRAME_SIZE)
		return -EINVAL;

	if (frame->picture_aspect > HDMI_PICTURE_ASPECT_16_9)
		return -EINVAL;

	return 0;
}

/**
 * hdmi_avi_infoframe_check() - check a HDMI AVI infoframe
 * @frame: HDMI AVI infoframe
 *
 * Validates that the infoframe is consistent and updates derived fields
 * (eg. length) based on other fields.
 *
 * Returns 0 on success or a negative error code on failure.
 */
int hdmi_avi_infoframe_check(struct hdmi_avi_infoframe *frame)
{
	return hdmi_avi_infoframe_check_only(frame);
}

/**
 * hdmi_avi_infoframe_pack_only() - write HDMI AVI infoframe to binary buffer
 * @frame: HDMI AVI infoframe
 * @buffer: destination buffer
 * @size: size of buffer
 *
 * Packs the information contained in the @frame structure into a binary
 * representation that can be written into the corresponding controller
 * registers. Also computes the checksum as required by section 5.3.5 of
 * the HDMI 1.4 specification.
 *
 * Returns the number of bytes packed into the binary buffer or a negative
 * error code on failure.
 */
ssize_t hdmi_avi_infoframe_pack_only(const struct hdmi_avi_infoframe *frame,
				     void *buffer, size_t size)
{
	u8 *ptr = buffer;
	size_t length;
	int ret;

	ret = hdmi_avi_infoframe_check_only(frame);
	if (ret)
		return ret;

	length = HDMI_INFOFRAME_HEADER_SIZE + frame->length;

	if (size < length)
		return -ENOSPC;

	memset(buffer, 0, size);

	ptr[0] = frame->type;
	ptr[1] = frame->version;
	ptr[2] = frame->length;
	ptr[3] = 0; /* checksum */

	/* start infoframe payload */
	ptr += HDMI_INFOFRAME_HEADER_SIZE;

	ptr[0] = ((frame->colorspace & 0x3) << 5) | (frame->scan_mode & 0x3);

	/*
	 * Data byte 1, bit 4 has to be set if we provide the active format
	 * aspect ratio
	 */
	if (frame->active_aspect & 0xf)
		ptr[0] |= BIT(4);

	/* Bit 3 and 2 indicate if we transmit horizontal/vertical bar data */
	if (frame->top_bar || frame->bottom_bar)
		ptr[0] |= BIT(3);

	if (frame->left_bar || frame->right_bar)
		ptr[0] |= BIT(2);

	ptr[1] = ((frame->colorimetry & 0x3) << 6) |
		 ((frame->picture_aspect & 0x3) << 4) |
		 (frame->active_aspect & 0xf);

	ptr[2] = ((frame->extended_colorimetry & 0x7) << 4) |
		 ((frame->quantization_range & 0x3) << 2) |
		 (frame->nups & 0x3);

	if (frame->itc)
		ptr[2] |= BIT(7);

	ptr[3] = frame->video_code & 0x7f;

	ptr[4] = ((frame->ycc_quantization_range & 0x3) << 6) |
		 ((frame->content_type & 0x3) << 4) |
		 (frame->pixel_repeat & 0xf);

	ptr[5] = frame->top_bar & 0xff;
	ptr[6] = (frame->top_bar >> 8) & 0xff;
	ptr[7] = frame->bottom_bar & 0xff;
	ptr[8] = (frame->bottom_bar >> 8) & 0xff;
	ptr[9] = frame->left_bar & 0xff;
	ptr[10] = (frame->left_bar >> 8) & 0xff;
	ptr[11] = frame->right_bar & 0xff;
	ptr[12] = (frame->right_bar >> 8) & 0xff;

	hdmi_infoframe_set_checksum(buffer, length);

	return length;
}

/**
 * hdmi_avi_infoframe_pack() - check a HDMI AVI infoframe,
 *                             and write it to binary buffer
 * @frame: HDMI AVI infoframe
 * @buffer: destination buffer
 * @size: size of buffer
 *
 * Validates that the infoframe is consistent and updates derived fields
 * (eg. length) based on other fields, after which it packs the information
 * contained in the @frame structure into a binary representation that
 * can be written into the corresponding controller registers. This function
 * also computes the checksum as required by section 5.3.5 of the HDMI 1.4
 * specification.
 *
 * Returns the number of bytes packed into the binary buffer or a negative
 * error code on failure.
 */
ssize_t hdmi_avi_infoframe_pack(struct hdmi_avi_infoframe *frame,
				void *buffer, size_t size)
{
	int ret;

	ret = hdmi_avi_infoframe_check(frame);
	if (ret)
		return ret;

	return hdmi_avi_infoframe_pack_only(frame, buffer, size);
}

/**
 * hdmi_spd_infoframe_init() - initialize an HDMI SPD infoframe
 * @frame: HDMI SPD infoframe
 * @vendor: vendor string
 * @product: product string
 *
 * Returns 0 on success or a negative error code on failure.
 */
int hdmi_spd_infoframe_init(struct hdmi_spd_infoframe *frame,
			    const char *vendor, const char *product)
{
	memset(frame, 0, sizeof(*frame));

	frame->type = HDMI_INFOFRAME_TYPE_SPD;
	frame->version = 1;
	frame->length = HDMI_SPD_INFOFRAME_SIZE;

	strncpy(frame->vendor, vendor, sizeof(frame->vendor));
	strncpy(frame->product, product, sizeof(frame->product));

	return 0;
}

static int hdmi_spd_infoframe_check_only(const struct hdmi_spd_infoframe *frame)
{
	if (frame->type != HDMI_INFOFRAME_TYPE_SPD ||
	    frame->version != 1 ||
	    frame->length != HDMI_SPD_INFOFRAME_SIZE)
		return -EINVAL;

	return 0;
}

/**
 * hdmi_spd_infoframe_check() - check a HDMI SPD infoframe
 * @frame: HDMI SPD infoframe
 *
 * Validates that the infoframe is consistent and updates derived fields
 * (eg. length) based on other fields.
 *
 * Returns 0 on success or a negative error code on failure.
 */
int hdmi_spd_infoframe_check(struct hdmi_spd_infoframe *frame)
{
	return hdmi_spd_infoframe_check_only(frame);
}

/**
 * hdmi_spd_infoframe_pack_only() - write HDMI SPD infoframe to binary buffer
 * @frame: HDMI SPD infoframe
 * @buffer: destination buffer
 * @size: size of buffer
 *
 * Packs the information contained in the @frame structure into a binary
 * representation that can be written into the corresponding controller
 * registers. Also computes the checksum as required by section 5.3.5 of
 * the HDMI 1.4 specification.
 *
 * Returns the number of bytes packed into the binary buffer or a negative
 * error code on failure.
 */
ssize_t hdmi_spd_infoframe_pack_only(const struct hdmi_spd_infoframe *frame,
				     void *buffer, size_t size)
{
	u8 *ptr = buffer;
	size_t length;
	int ret;

	ret = hdmi_spd_infoframe_check_only(frame);
	if (ret)
		return ret;

	length = HDMI_INFOFRAME_HEADER_SIZE + frame->length;

	if (size < length)
		return -ENOSPC;

	memset(buffer, 0, size);

	ptr[0] = frame->type;
	ptr[1] = frame->version;
	ptr[2] = frame->length;
	ptr[3] = 0; /* checksum */

	/* start infoframe payload */
	ptr += HDMI_INFOFRAME_HEADER_SIZE;

	memcpy(ptr, frame->vendor, sizeof(frame->vendor));
	memcpy(ptr + 8, frame->product, sizeof(frame->product));

	ptr[24] = frame->sdi;

	hdmi_infoframe_set_checksum(buffer, length);

	return length;
}

/**
 * hdmi_spd_infoframe_pack() - check a HDMI SPD infoframe,
 *                             and write it to binary buffer
 * @frame: HDMI SPD infoframe
 * @buffer: destination buffer
 * @size: size of buffer
 *
 * Validates that the infoframe is consistent and updates derived fields
 * (eg. length) based on other fields, after which it packs the information
 * contained in the @frame structure into a binary representation that
 * can be written into the corresponding controller registers. This function
 * also computes the checksum as required by section 5.3.5 of the HDMI 1.4
 * specification.
 *
 * Returns the number of bytes packed into the binary buffer or a negative
 * error code on failure.
 */
ssize_t hdmi_spd_infoframe_pack(struct hdmi_spd_infoframe *frame,
				void *buffer, size_t size)
{
	int ret;

	ret = hdmi_spd_infoframe_check(frame);
	if (ret)
		return ret;

	return hdmi_spd_infoframe_pack_only(frame, buffer, size);
}

/**
 * hdmi_audio_infoframe_init() - initialize an HDMI audio infoframe
 * @frame: HDMI audio infoframe
 *
 * Returns 0 on success or a negative error code on failure.
 */
int hdmi_audio_infoframe_init(struct hdmi_audio_infoframe *frame)
{
	memset(frame, 0, sizeof(*frame));

	frame->type = HDMI_INFOFRAME_TYPE_AUDIO;
	frame->version = 1;
	frame->length = HDMI_AUDIO_INFOFRAME_SIZE;

	return 0;
}

static int hdmi_audio_infoframe_check_only(const struct hdmi_audio_infoframe *frame)
{
	if (frame->type != HDMI_INFOFRAME_TYPE_AUDIO ||
	    frame->version != 1 ||
	    frame->length != HDMI_AUDIO_INFOFRAME_SIZE)
		return -EINVAL;

	return 0;
}

/**
 * hdmi_audio_infoframe_check() - check a HDMI audio infoframe
 * @frame: HDMI audio infoframe
 *
 * Validates that the infoframe is consistent and updates derived fields
 * (eg. length) based on other fields.
 *
 * Returns 0 on success or a negative error code on failure.
 */
int hdmi_audio_infoframe_check(struct hdmi_audio_infoframe *frame)
{
	return hdmi_audio_infoframe_check_only(frame);
}

/**
 * hdmi_audio_infoframe_pack_only() - write HDMI audio infoframe to binary buffer
 * @frame: HDMI audio infoframe
 * @buffer: destination buffer
 * @size: size of buffer
 *
 * Packs the information contained in the @frame structure into a binary
 * representation that can be written into the corresponding controller
 * registers. Also computes the checksum as required by section 5.3.5 of
 * the HDMI 1.4 specification.
 *
 * Returns the number of bytes packed into the binary buffer or a negative
 * error code on failure.
 */
ssize_t hdmi_audio_infoframe_pack_only(const struct hdmi_audio_infoframe *frame,
				       void *buffer, size_t size)
{
	unsigned char channels;
	u8 *ptr = buffer;
	size_t length;
	int ret;

	ret = hdmi_audio_infoframe_check_only(frame);
	if (ret)
		return ret;

	length = HDMI_INFOFRAME_HEADER_SIZE + frame->length;

	if (size < length)
		return -ENOSPC;

	memset(buffer, 0, size);

	if (frame->channels >= 2)
		channels = frame->channels - 1;
	else
		channels = 0;

	ptr[0] = frame->type;
	ptr[1] = frame->version;
	ptr[2] = frame->length;
	ptr[3] = 0; /* checksum */

	/* start infoframe payload */
	ptr += HDMI_INFOFRAME_HEADER_SIZE;

	ptr[0] = ((frame->coding_type & 0xf) << 4) | (channels & 0x7);
	ptr[1] = ((frame->sample_frequency & 0x7) << 2) |
		 (frame->sample_size & 0x3);
	ptr[2] = frame->coding_type_ext & 0x1f;
	ptr[3] = frame->channel_allocation;
	ptr[4] = (frame->level_shift_value & 0xf) << 3;

	if (frame->downmix_inhibit)
		ptr[4] |= BIT(7);

	hdmi_infoframe_set_checksum(buffer, length);

	return length;
}

/**
 * hdmi_audio_infoframe_pack() - check a HDMI Audio infoframe,
 *                               and write it to binary buffer
 * @frame: HDMI Audio infoframe
 * @buffer: destination buffer
 * @size: size of buffer
 *
 * Validates that the infoframe is consistent and updates derived fields
 * (eg. length) based on other fields, after which it packs the information
 * contained in the @frame structure into a binary representation that
 * can be written into the corresponding controller registers. This function
 * also computes the checksum as required by section 5.3.5 of the HDMI 1.4
 * specification.
 *
 * Returns the number of bytes packed into the binary buffer or a negative
 * error code on failure.
 */
ssize_t hdmi_audio_infoframe_pack(struct hdmi_audio_infoframe *frame,
				  void *buffer, size_t size)
{
	int ret;

	ret = hdmi_audio_infoframe_check(frame);
	if (ret)
		return ret;

	return hdmi_audio_infoframe_pack_only(frame, buffer, size);
}

/**
 * hdmi_vendor_infoframe_init() - initialize an HDMI vendor infoframe
 * @frame: HDMI vendor infoframe
 *
 * Returns 0 on success or a negative error code on failure.
 */
int hdmi_vendor_infoframe_init(struct hdmi_vendor_infoframe *frame)
{
	memset(frame, 0, sizeof(*frame));

	frame->type = HDMI_INFOFRAME_TYPE_VENDOR;
	frame->version = 1;

	frame->oui = HDMI_IEEE_OUI;

	/*
	 * 0 is a valid value for s3d_struct, so we use a special "not set"
	 * value
	 */
	frame->s3d_struct = HDMI_3D_STRUCTURE_INVALID;
	frame->length = 4;

	return 0;
}

static int hdmi_vendor_infoframe_length(const struct hdmi_vendor_infoframe *frame)
{
	/* for side by side (half) we also need to provide 3D_Ext_Data */
	if (frame->s3d_struct >= HDMI_3D_STRUCTURE_SIDE_BY_SIDE_HALF)
		return 6;
	else if (frame->vic != 0 || frame->s3d_struct != HDMI_3D_STRUCTURE_INVALID)
		return 5;
	else
		return 4;
}

static int hdmi_vendor_infoframe_check_only(const struct hdmi_vendor_infoframe *frame)
{
	if (frame->type != HDMI_INFOFRAME_TYPE_VENDOR ||
	    frame->version != 1 ||
	    frame->oui != HDMI_IEEE_OUI)
		return -EINVAL;

	/* only one of those can be supplied */
	if (frame->vic != 0 && frame->s3d_struct != HDMI_3D_STRUCTURE_INVALID)
		return -EINVAL;

	if (frame->length != hdmi_vendor_infoframe_length(frame))
		return -EINVAL;

	return 0;
}

/**
 * hdmi_vendor_infoframe_check() - check a HDMI vendor infoframe
 * @frame: HDMI infoframe
 *
 * Validates that the infoframe is consistent and updates derived fields
 * (eg. length) based on other fields.
 *
 * Returns 0 on success or a negative error code on failure.
 */
int hdmi_vendor_infoframe_check(struct hdmi_vendor_infoframe *frame)
{
	frame->length = hdmi_vendor_infoframe_length(frame);

	return hdmi_vendor_infoframe_check_only(frame);
}

/**
 * hdmi_vendor_infoframe_pack_only() - write a HDMI vendor infoframe to binary buffer
 * @frame: HDMI infoframe
 * @buffer: destination buffer
 * @size: size of buffer
 *
 * Packs the information contained in the @frame structure into a binary
 * representation that can be written into the corresponding controller
 * registers. Also computes the checksum as required by section 5.3.5 of
 * the HDMI 1.4 specification.
 *
 * Returns the number of bytes packed into the binary buffer or a negative
 * error code on failure.
 */
ssize_t hdmi_vendor_infoframe_pack_only(const struct hdmi_vendor_infoframe *frame,
					void *buffer, size_t size)
{
	u8 *ptr = buffer;
	size_t length;
	int ret;

	ret = hdmi_vendor_infoframe_check_only(frame);
	if (ret)
		return ret;

	length = HDMI_INFOFRAME_HEADER_SIZE + frame->length;

	if (size < length)
		return -ENOSPC;

	memset(buffer, 0, size);

	ptr[0] = frame->type;
	ptr[1] = frame->version;
	ptr[2] = frame->length;
	ptr[3] = 0; /* checksum */

	/* HDMI OUI */
	ptr[4] = 0x03;
	ptr[5] = 0x0c;
	ptr[6] = 0x00;

	if (frame->s3d_struct != HDMI_3D_STRUCTURE_INVALID) {
		ptr[7] = 0x2 << 5;	/* video format */
		ptr[8] = (frame->s3d_struct & 0xf) << 4;
		if (frame->s3d_struct >= HDMI_3D_STRUCTURE_SIDE_BY_SIDE_HALF)
			ptr[9] = (frame->s3d_ext_data & 0xf) << 4;
	} else if (frame->vic) {
		ptr[7] = 0x1 << 5;	/* video format */
		ptr[8] = frame->vic;
	} else {
		ptr[7] = 0x0 << 5;	/* video format */
	}

	hdmi_infoframe_set_checksum(buffer, length);

	return length;
}

/**
 * hdmi_vendor_infoframe_pack() - check a HDMI Vendor infoframe,
 *                                and write it to binary buffer
 * @frame: HDMI Vendor infoframe
 * @buffer: destination buffer
 * @size: size of buffer
 *
 * Validates that the infoframe is consistent and updates derived fields
 * (eg. length) based on other fields, after which it packs the information
 * contained in the @frame structure into a binary representation that
 * can be written into the corresponding controller registers. This function
 * also computes the checksum as required by section 5.3.5 of the HDMI 1.4
 * specification.
 *
 * Returns the number of bytes packed into the binary buffer or a negative
 * error code on failure.
 */
ssize_t hdmi_vendor_infoframe_pack(struct hdmi_vendor_infoframe *frame,
				   void *buffer, size_t size)
{
	int ret;

	ret = hdmi_vendor_infoframe_check(frame);
	if (ret)
		return ret;

	return hdmi_vendor_infoframe_pack_only(frame, buffer, size);
}

static int
hdmi_vendor_any_infoframe_check_only(const union hdmi_vendor_any_infoframe *frame)
{
	if (frame->any.type != HDMI_INFOFRAME_TYPE_VENDOR ||
	    frame->any.version != 1)
		return -EINVAL;

	return 0;
}

/**
 * hdmi_drm_infoframe_init() - initialize an HDMI dynamic Range and
 * mastering infoframe
 * @frame: HDMI DRM infoframe
 *
 * Returns 0 on success or a negative error code on failure.
 */
int hdmi_drm_infoframe_init(struct hdmi_drm_infoframe *frame)
{
	memset(frame, 0, sizeof(*frame));

	frame->type = HDMI_INFOFRAME_TYPE_DRM;
	frame->version = 1;
	frame->length = HDMI_DRM_INFOFRAME_SIZE;

	return 0;
}

static int hdmi_drm_infoframe_check_only(const struct hdmi_drm_infoframe *frame)
{
	if (frame->type != HDMI_INFOFRAME_TYPE_DRM ||
	    frame->version != 1)
		return -EINVAL;

	if (frame->length != HDMI_DRM_INFOFRAME_SIZE)
		return -EINVAL;

	return 0;
}

/**
 * hdmi_drm_infoframe_check() - check a HDMI DRM infoframe
 * @frame: HDMI DRM infoframe
 *
 * Validates that the infoframe is consistent.
 * Returns 0 on success or a negative error code on failure.
 */
int hdmi_drm_infoframe_check(struct hdmi_drm_infoframe *frame)
{
	return hdmi_drm_infoframe_check_only(frame);
}

/**
 * hdmi_drm_infoframe_pack_only() - write HDMI DRM infoframe to binary buffer
 * @frame: HDMI DRM infoframe
 * @buffer: destination buffer
 * @size: size of buffer
 *
 * Packs the information contained in the @frame structure into a binary
 * representation that can be written into the corresponding controller
 * registers. Also computes the checksum as required by section 5.3.5 of
 * the HDMI 1.4 specification.
 *
 * Returns the number of bytes packed into the binary buffer or a negative
 * error code on failure.
 */
ssize_t hdmi_drm_infoframe_pack_only(const struct hdmi_drm_infoframe *frame,
				     void *buffer, size_t size)
{
	u8 *ptr = buffer;
	size_t length;
	int i;

	length = HDMI_INFOFRAME_HEADER_SIZE + frame->length;

	if (size < length)
		return -ENOSPC;

	memset(buffer, 0, size);

	ptr[0] = frame->type;
	ptr[1] = frame->version;
	ptr[2] = frame->length;
	ptr[3] = 0; /* checksum */

	/* start infoframe payload */
	ptr += HDMI_INFOFRAME_HEADER_SIZE;

	*ptr++ = frame->eotf;
	*ptr++ = frame->metadata_type;

	for (i = 0; i < 3; i++) {
		*ptr++ = frame->display_primaries[i].x;
		*ptr++ = frame->display_primaries[i].x >> 8;
		*ptr++ = frame->display_primaries[i].y;
		*ptr++ = frame->display_primaries[i].y >> 8;
	}

	*ptr++ = frame->white_point.x;
	*ptr++ = frame->white_point.x >> 8;

	*ptr++ = frame->white_point.y;
	*ptr++ = frame->white_point.y >> 8;

	*ptr++ = frame->max_display_mastering_luminance;
	*ptr++ = frame->max_display_mastering_luminance >> 8;

	*ptr++ = frame->min_display_mastering_luminance;
	*ptr++ = frame->min_display_mastering_luminance >> 8;

	*ptr++ = frame->max_cll;
	*ptr++ = frame->max_cll >> 8;

	*ptr++ = frame->max_fall;
	*ptr++ = frame->max_fall >> 8;

	hdmi_infoframe_set_checksum(buffer, length);

	return length;
}

/**
 * hdmi_drm_infoframe_pack() - check a HDMI DRM infoframe,
 *                             and write it to binary buffer
 * @frame: HDMI DRM infoframe
 * @buffer: destination buffer
 * @size: size of buffer
 *
 * Validates that the infoframe is consistent and updates derived fields
 * (eg. length) based on other fields, after which it packs the information
 * contained in the @frame structure into a binary representation that
 * can be written into the corresponding controller registers. This function
 * also computes the checksum as required by section 5.3.5 of the HDMI 1.4
 * specification.
 *
 * Returns the number of bytes packed into the binary buffer or a negative
 * error code on failure.
 */
ssize_t hdmi_drm_infoframe_pack(struct hdmi_drm_infoframe *frame,
				void *buffer, size_t size)
{
	int ret;

	ret = hdmi_drm_infoframe_check(frame);
	if (ret)
		return ret;

	return hdmi_drm_infoframe_pack_only(frame, buffer, size);
}

/*
 * hdmi_vendor_any_infoframe_check() - check a vendor infoframe
 */
static int
hdmi_vendor_any_infoframe_check(union hdmi_vendor_any_infoframe *frame)
{
	int ret;

	ret = hdmi_vendor_any_infoframe_check_only(frame);
	if (ret)
		return ret;

	/* we only know about HDMI vendor infoframes */
	if (frame->any.oui != HDMI_IEEE_OUI)
		return -EINVAL;

	return hdmi_vendor_infoframe_check(&frame->hdmi);
}

/*
 * hdmi_vendor_any_infoframe_pack_only() - write a vendor infoframe to binary buffer
 */
static ssize_t
hdmi_vendor_any_infoframe_pack_only(const union hdmi_vendor_any_infoframe *frame,
				    void *buffer, size_t size)
{
	int ret;

	ret = hdmi_vendor_any_infoframe_check_only(frame);
	if (ret)
		return ret;

	/* we only know about HDMI vendor infoframes */
	if (frame->any.oui != HDMI_IEEE_OUI)
		return -EINVAL;

	return hdmi_vendor_infoframe_pack_only(&frame->hdmi, buffer, size);
}

/*
 * hdmi_vendor_any_infoframe_pack() - check a vendor infoframe,
 *                                    and write it to binary buffer
 */
static ssize_t
hdmi_vendor_any_infoframe_pack(union hdmi_vendor_any_infoframe *frame,
			       void *buffer, size_t size)
{
	int ret;

	ret = hdmi_vendor_any_infoframe_check(frame);
	if (ret)
		return ret;

	return hdmi_vendor_any_infoframe_pack_only(frame, buffer, size);
}

/**
 * hdmi_infoframe_check() - check a HDMI infoframe
 * @frame: HDMI infoframe
 *
 * Validates that the infoframe is consistent and updates derived fields
 * (eg. length) based on other fields.
 *
 * Returns 0 on success or a negative error code on failure.
 */
int
hdmi_infoframe_check(union hdmi_infoframe *frame)
{
	switch (frame->any.type) {
	case HDMI_INFOFRAME_TYPE_AVI:
		return hdmi_avi_infoframe_check(&frame->avi);
	case HDMI_INFOFRAME_TYPE_SPD:
		return hdmi_spd_infoframe_check(&frame->spd);
	case HDMI_INFOFRAME_TYPE_AUDIO:
		return hdmi_audio_infoframe_check(&frame->audio);
	case HDMI_INFOFRAME_TYPE_VENDOR:
		return hdmi_vendor_any_infoframe_check(&frame->vendor);
	default:
		WARN(1, "Bad infoframe type %d\n", frame->any.type);
		return -EINVAL;
	}
}

/**
 * hdmi_infoframe_pack_only() - write a HDMI infoframe to binary buffer
 * @frame: HDMI infoframe
 * @buffer: destination buffer
 * @size: size of buffer
 *
 * Packs the information contained in the @frame structure into a binary
 * representation that can be written into the corresponding controller
 * registers. Also computes the checksum as required by section 5.3.5 of
 * the HDMI 1.4 specification.
 *
 * Returns the number of bytes packed into the binary buffer or a negative
 * error code on failure.
 */
ssize_t
hdmi_infoframe_pack_only(const union hdmi_infoframe *frame, void *buffer, size_t size)
{
	ssize_t length;

	switch (frame->any.type) {
	case HDMI_INFOFRAME_TYPE_AVI:
		length = hdmi_avi_infoframe_pack_only(&frame->avi,
						      buffer, size);
		break;
	case HDMI_INFOFRAME_TYPE_DRM:
		length = hdmi_drm_infoframe_pack_only(&frame->drm,
						      buffer, size);
		break;
	case HDMI_INFOFRAME_TYPE_SPD:
		length = hdmi_spd_infoframe_pack_only(&frame->spd,
						      buffer, size);
		break;
	case HDMI_INFOFRAME_TYPE_AUDIO:
		length = hdmi_audio_infoframe_pack_only(&frame->audio,
							buffer, size);
		break;
	case HDMI_INFOFRAME_TYPE_VENDOR:
		length = hdmi_vendor_any_infoframe_pack_only(&frame->vendor,
							     buffer, size);
		break;
	default:
		WARN(1, "Bad infoframe type %d\n", frame->any.type);
		length = -EINVAL;
	}

	return length;
}

/**
 * hdmi_infoframe_pack() - check a HDMI infoframe,
 *                         and write it to binary buffer
 * @frame: HDMI infoframe
 * @buffer: destination buffer
 * @size: size of buffer
 *
 * Validates that the infoframe is consistent and updates derived fields
 * (eg. length) based on other fields, after which it packs the information
 * contained in the @frame structure into a binary representation that
 * can be written into the corresponding controller registers. This function
 * also computes the checksum as required by section 5.3.5 of the HDMI 1.4
 * specification.
 *
 * Returns the number of bytes packed into the binary buffer or a negative
 * error code on failure.
 */
ssize_t
hdmi_infoframe_pack(union hdmi_infoframe *frame,
		    void *buffer, size_t size)
{
	ssize_t length;

	switch (frame->any.type) {
	case HDMI_INFOFRAME_TYPE_AVI:
		length = hdmi_avi_infoframe_pack(&frame->avi, buffer, size);
		break;
	case HDMI_INFOFRAME_TYPE_DRM:
		length = hdmi_drm_infoframe_pack(&frame->drm, buffer, size);
		break;
	case HDMI_INFOFRAME_TYPE_SPD:
		length = hdmi_spd_infoframe_pack(&frame->spd, buffer, size);
		break;
	case HDMI_INFOFRAME_TYPE_AUDIO:
		length = hdmi_audio_infoframe_pack(&frame->audio, buffer, size);
		break;
	case HDMI_INFOFRAME_TYPE_VENDOR:
		length = hdmi_vendor_any_infoframe_pack(&frame->vendor,
							buffer, size);
		break;
	default:
		WARN(1, "Bad infoframe type %d\n", frame->any.type);
		length = -EINVAL;
	}

	return length;
}

/**
 * hdmi_avi_infoframe_unpack() - unpack binary buffer to a HDMI AVI infoframe
 * @frame: HDMI AVI infoframe
 * @buffer: source buffer
 * @size: size of buffer
 *
 * Unpacks the information contained in binary @buffer into a structured
 * @frame of the HDMI Auxiliary Video (AVI) information frame.
 * Also verifies the checksum as required by section 5.3.5 of the HDMI 1.4
 * specification.
 *
 * Returns 0 on success or a negative error code on failure.
 */
static int hdmi_avi_infoframe_unpack(struct hdmi_avi_infoframe *frame,
				     const void *buffer, size_t size)
{
	const u8 *ptr = buffer;
	int ret;

	if (size < HDMI_INFOFRAME_SIZE(AVI))
		return -EINVAL;

	if (ptr[0] != HDMI_INFOFRAME_TYPE_AVI ||
	    ptr[1] != 2 ||
	    ptr[2] != HDMI_AVI_INFOFRAME_SIZE)
		return -EINVAL;

	if (hdmi_infoframe_checksum(buffer, HDMI_INFOFRAME_SIZE(AVI)) != 0)
		return -EINVAL;

	ret = hdmi_avi_infoframe_init(frame);
	if (ret)
		return ret;

	ptr += HDMI_INFOFRAME_HEADER_SIZE;

	frame->colorspace = (ptr[0] >> 5) & 0x3;
	if (ptr[0] & 0x10)
		frame->active_aspect = ptr[1] & 0xf;
	if (ptr[0] & 0x8) {
		frame->top_bar = (ptr[6] << 8) | ptr[5];
		frame->bottom_bar = (ptr[8] << 8) | ptr[7];
	}
	if (ptr[0] & 0x4) {
		frame->left_bar = (ptr[10] << 8) | ptr[9];
		frame->right_bar = (ptr[12] << 8) | ptr[11];
	}
	frame->scan_mode = ptr[0] & 0x3;

	frame->colorimetry = (ptr[1] >> 6) & 0x3;
	frame->picture_aspect = (ptr[1] >> 4) & 0x3;
	frame->active_aspect = ptr[1] & 0xf;

	frame->itc = ptr[2] & 0x80 ? true : false;
	frame->extended_colorimetry = (ptr[2] >> 4) & 0x7;
	frame->quantization_range = (ptr[2] >> 2) & 0x3;
	frame->nups = ptr[2] & 0x3;

	frame->video_code = ptr[3] & 0x7f;
	frame->ycc_quantization_range = (ptr[4] >> 6) & 0x3;
	frame->content_type = (ptr[4] >> 4) & 0x3;

	frame->pixel_repeat = ptr[4] & 0xf;

	return 0;
}

/**
 * hdmi_spd_infoframe_unpack() - unpack binary buffer to a HDMI SPD infoframe
 * @frame: HDMI SPD infoframe
 * @buffer: source buffer
 * @size: size of buffer
 *
 * Unpacks the information contained in binary @buffer into a structured
 * @frame of the HDMI Source Product Description (SPD) information frame.
 * Also verifies the checksum as required by section 5.3.5 of the HDMI 1.4
 * specification.
 *
 * Returns 0 on success or a negative error code on failure.
 */
static int hdmi_spd_infoframe_unpack(struct hdmi_spd_infoframe *frame,
				     const void *buffer, size_t size)
{
	const char *ptr = buffer;
	int ret;

	if (size < HDMI_INFOFRAME_SIZE(SPD))
		return -EINVAL;

	if (ptr[0] != HDMI_INFOFRAME_TYPE_SPD ||
	    ptr[1] != 1 ||
	    ptr[2] != HDMI_SPD_INFOFRAME_SIZE) {
		return -EINVAL;
	}

	if (hdmi_infoframe_checksum(buffer, HDMI_INFOFRAME_SIZE(SPD)) != 0)
		return -EINVAL;

	ptr += HDMI_INFOFRAME_HEADER_SIZE;

	ret = hdmi_spd_infoframe_init(frame, ptr, ptr + 8);
	if (ret)
		return ret;

	frame->sdi = ptr[24];

	return 0;
}

/**
 * hdmi_audio_infoframe_unpack() - unpack binary buffer to a HDMI AUDIO infoframe
 * @frame: HDMI Audio infoframe
 * @buffer: source buffer
 * @size: size of buffer
 *
 * Unpacks the information contained in binary @buffer into a structured
 * @frame of the HDMI Audio information frame.
 * Also verifies the checksum as required by section 5.3.5 of the HDMI 1.4
 * specification.
 *
 * Returns 0 on success or a negative error code on failure.
 */
static int hdmi_audio_infoframe_unpack(struct hdmi_audio_infoframe *frame,
				       const void *buffer, size_t size)
{
	const u8 *ptr = buffer;
	int ret;

	if (size < HDMI_INFOFRAME_SIZE(AUDIO))
		return -EINVAL;

	if (ptr[0] != HDMI_INFOFRAME_TYPE_AUDIO ||
	    ptr[1] != 1 ||
	    ptr[2] != HDMI_AUDIO_INFOFRAME_SIZE) {
		return -EINVAL;
	}

	if (hdmi_infoframe_checksum(buffer, HDMI_INFOFRAME_SIZE(AUDIO)) != 0)
		return -EINVAL;

	ret = hdmi_audio_infoframe_init(frame);
	if (ret)
		return ret;

	ptr += HDMI_INFOFRAME_HEADER_SIZE;

	frame->channels = ptr[0] & 0x7;
	frame->coding_type = (ptr[0] >> 4) & 0xf;
	frame->sample_size = ptr[1] & 0x3;
	frame->sample_frequency = (ptr[1] >> 2) & 0x7;
	frame->coding_type_ext = ptr[2] & 0x1f;
	frame->channel_allocation = ptr[3];
	frame->level_shift_value = (ptr[4] >> 3) & 0xf;
	frame->downmix_inhibit = ptr[4] & 0x80 ? true : false;

	return 0;
}

/**
 * hdmi_vendor_infoframe_unpack() - unpack binary buffer to a HDMI vendor infoframe
 * @frame: HDMI Vendor infoframe
 * @buffer: source buffer
 * @size: size of buffer
 *
 * Unpacks the information contained in binary @buffer into a structured
 * @frame of the HDMI Vendor information frame.
 * Also verifies the checksum as required by section 5.3.5 of the HDMI 1.4
 * specification.
 *
 * Returns 0 on success or a negative error code on failure.
 */
static int
hdmi_vendor_any_infoframe_unpack(union hdmi_vendor_any_infoframe *frame,
				 const void *buffer, size_t size)
{
	const u8 *ptr = buffer;
	size_t length;
	int ret;
	u8 hdmi_video_format;
	struct hdmi_vendor_infoframe *hvf = &frame->hdmi;

	if (size < HDMI_INFOFRAME_HEADER_SIZE)
		return -EINVAL;

	if (ptr[0] != HDMI_INFOFRAME_TYPE_VENDOR ||
	    ptr[1] != 1 ||
	    (ptr[2] != 4 && ptr[2] != 5 && ptr[2] != 6))
		return -EINVAL;

	length = ptr[2];

	if (size < HDMI_INFOFRAME_HEADER_SIZE + length)
		return -EINVAL;

	if (hdmi_infoframe_checksum(buffer,
				    HDMI_INFOFRAME_HEADER_SIZE + length) != 0)
		return -EINVAL;

	ptr += HDMI_INFOFRAME_HEADER_SIZE;

	/* HDMI OUI */
	if (ptr[0] != 0x03 || ptr[1] != 0x0c || ptr[2] != 0x00)
		return -EINVAL;

	hdmi_video_format = ptr[3] >> 5;

	if (hdmi_video_format > 0x2)
		return -EINVAL;

	ret = hdmi_vendor_infoframe_init(hvf);
	if (ret)
		return ret;

	hvf->length = length;

	if (hdmi_video_format == 0x2) {
		if (length != 5 && length != 6)
			return -EINVAL;
		hvf->s3d_struct = ptr[4] >> 4;
		if (hvf->s3d_struct >= HDMI_3D_STRUCTURE_SIDE_BY_SIDE_HALF) {
			if (length != 6)
				return -EINVAL;
			hvf->s3d_ext_data = ptr[5] >> 4;
		}
	} else if (hdmi_video_format == 0x1) {
		if (length != 5)
			return -EINVAL;
		hvf->vic = ptr[4];
	} else {
		if (length != 4)
			return -EINVAL;
	}

	return 0;
}

/**
 * hdmi_drm_infoframe_unpack() - unpack binary buffer to a HDMI DRM infoframe
 * @frame: HDMI DRM infoframe
 * @buffer: source buffer
 * @size: size of buffer
 *
 * Unpacks the information contained in binary @buffer into a structured
 * @frame of the HDMI Dynamic Range and Mastering (DRM) information frame.
 * Also verifies the checksum as required by section 5.3.5 of the HDMI 1.4
 * specification.
 *
 * Returns 0 on success or a negative error code on failure.
 */
static int hdmi_drm_infoframe_unpack(struct hdmi_drm_infoframe *frame,
				     const void *buffer, size_t size)
{
	const u8 *ptr = buffer;
	const u8 *temp;
	u8 x_lsb, x_msb;
	u8 y_lsb, y_msb;
	int ret;
	int i;

	if (size < HDMI_INFOFRAME_SIZE(DRM))
		return -EINVAL;

	if (ptr[0] != HDMI_INFOFRAME_TYPE_DRM ||
	    ptr[1] != 1 ||
	    ptr[2] != HDMI_DRM_INFOFRAME_SIZE)
		return -EINVAL;

	if (hdmi_infoframe_checksum(buffer, HDMI_INFOFRAME_SIZE(DRM)) != 0)
		return -EINVAL;

	ret = hdmi_drm_infoframe_init(frame);
	if (ret)
		return ret;

	ptr += HDMI_INFOFRAME_HEADER_SIZE;

	frame->eotf = ptr[0] & 0x7;
	frame->metadata_type = ptr[1] & 0x7;

	temp = ptr + 2;
	for (i = 0; i < 3; i++) {
		x_lsb = *temp++;
		x_msb = *temp++;
		frame->display_primaries[i].x =  (x_msb << 8) | x_lsb;
		y_lsb = *temp++;
		y_msb = *temp++;
		frame->display_primaries[i].y = (y_msb << 8) | y_lsb;
	}

	frame->white_point.x = (ptr[15] << 8) | ptr[14];
	frame->white_point.y = (ptr[17] << 8) | ptr[16];

	frame->max_display_mastering_luminance = (ptr[19] << 8) | ptr[18];
	frame->min_display_mastering_luminance = (ptr[21] << 8) | ptr[20];
	frame->max_cll = (ptr[23] << 8) | ptr[22];
	frame->max_fall = (ptr[25] << 8) | ptr[24];

	return 0;
}

/**
 * hdmi_infoframe_unpack() - unpack binary buffer to a HDMI infoframe
 * @frame: HDMI infoframe
 * @buffer: source buffer
 * @size: size of buffer
 *
 * Unpacks the information contained in binary buffer @buffer into a structured
 * @frame of a HDMI infoframe.
 * Also verifies the checksum as required by section 5.3.5 of the HDMI 1.4
 * specification.
 *
 * Returns 0 on success or a negative error code on failure.
 */
int hdmi_infoframe_unpack(union hdmi_infoframe *frame,
			  const void *buffer, size_t size)
{
	int ret;
	const u8 *ptr = buffer;

	if (size < HDMI_INFOFRAME_HEADER_SIZE)
		return -EINVAL;

	switch (ptr[0]) {
	case HDMI_INFOFRAME_TYPE_AVI:
		ret = hdmi_avi_infoframe_unpack(&frame->avi, buffer, size);
		break;
	case HDMI_INFOFRAME_TYPE_DRM:
		ret = hdmi_drm_infoframe_unpack(&frame->drm, buffer, size);
		break;
	case HDMI_INFOFRAME_TYPE_SPD:
		ret = hdmi_spd_infoframe_unpack(&frame->spd, buffer, size);
		break;
	case HDMI_INFOFRAME_TYPE_AUDIO:
		ret = hdmi_audio_infoframe_unpack(&frame->audio, buffer, size);
		break;
	case HDMI_INFOFRAME_TYPE_VENDOR:
		ret = hdmi_vendor_any_infoframe_unpack(&frame->vendor, buffer, size);
		break;
	default:
		ret = -EINVAL;
		break;
	}

	return ret;
}
