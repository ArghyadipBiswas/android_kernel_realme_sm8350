/***************************************************************
** Copyright (C),  2020,  OPLUS Mobile Comm Corp.,  Ltd
** VENDOR_EDIT
** File : oplus_display_panel_seed.c
** Description : oplus display panel seed feature
** Version : 1.0
** Date : 2020/06/13
**
** ------------------------------- Revision History: -----------
**  <author>        <data>        <version >        <desc>
**  Li.Sheng       2020/06/13        1.0           Build this moudle
******************************************************************/
#include "oplus_display_panel_seed.h"
#include "oplus_dsi_support.h"

int seed_mode = 0;

extern int oplus_seed_backlight;
extern int oplus_seed_last_backlight;
extern bool oplus_set_color_mode;
//#ifdef OPLUS_BUG_STABILITY
bool seed_mode_flag = false;
extern u32 bl_lvl_backup;
extern int oplus_dc_v2_on;
//#endif /*OPLUS_BUG_STABILITY*/

#define PANEL_LOADING_EFFECT_FLAG 100
#define PANEL_LOADING_EFFECT_MODE1 101
#define PANEL_LOADING_EFFECT_MODE2 102
#define PANEL_LOADING_EFFECT_OFF 100

DEFINE_MUTEX(oplus_seed_lock);

int oplus_display_get_seed_mode(void)
{
	return seed_mode;
}

int __oplus_display_set_seed(int mode)
{
	mutex_lock(&oplus_seed_lock);
	//#ifdef OPLUS_BUG_STABILITY
	if (bl_lvl_backup == 0 && oplus_dc_v2_on == false && seed_mode == 1) {
		seed_mode_flag = true;
        }
	//#endif /*OPLUS_BUG_STABILITY*/
	if (mode != seed_mode) {
		seed_mode = mode;
	}
	oplus_set_color_mode = true;

	mutex_unlock(&oplus_seed_lock);
	return 0;
}

extern int g_fingerprint_mode;
int dsi_panel_seed_mode_unlock(struct dsi_panel *panel, int mode)
{
	int rc = 0;

	if (!dsi_panel_initialized(panel)) {
		return -EINVAL;
	}
	//#ifdef OPLUS_BUG_STABILITY
	if (oplus_dc_v2_on) {
		if (seed_mode_flag  == true) {
			seed_mode = 1;
                        mode = 1;
			seed_mode_flag = false;
		}
	}
	//#endif /*OPLUS_BUG_STABILITY*/
	switch (mode) {
	case 0:
		if (!oplus_set_color_mode && oplus_seed_backlight && !strcmp(panel->name, "samsung AMS643YE01 dsc cmd mode panel")) {
			if (oplus_seed_backlight != oplus_seed_last_backlight) {
				rc = dsi_panel_tx_cmd_set(panel, DSI_CMD_SEED_DC_MODE0);
				oplus_seed_last_backlight = oplus_seed_backlight;
			}
		}
		else if (oplus_dc_v2_on) {
			if (g_fingerprint_mode)
				rc = dsi_panel_tx_cmd_set(panel, DSI_CMD_SEED_MODE0);
			else
				rc = dsi_panel_tx_cmd_set(panel, DSI_CMD_SEED_DC_MODE0);
		}
		else {
			rc = dsi_panel_tx_cmd_set(panel, DSI_CMD_SEED_MODE0);
		}

		if (rc) {
			pr_err("[%s] failed to send DSI_CMD_SEED_MODE0 cmds, rc=%d\n",
			       panel->name, rc);
		}

		break;

	case 1:
		if (!oplus_set_color_mode && oplus_seed_backlight && !strcmp(panel->name, "samsung AMS643YE01 dsc cmd mode panel")) {
			if (oplus_seed_backlight != oplus_seed_last_backlight) {
				rc = dsi_panel_tx_cmd_set(panel, DSI_CMD_SEED_DC_MODE1);
				oplus_seed_last_backlight = oplus_seed_backlight;
			}
		}
		else if (oplus_dc_v2_on) {
			if (g_fingerprint_mode)
				rc = dsi_panel_tx_cmd_set(panel, DSI_CMD_SEED_MODE1);
			else
				rc = dsi_panel_tx_cmd_set(panel, DSI_CMD_SEED_DC_MODE1);
		}
		else {
			rc = dsi_panel_tx_cmd_set(panel, DSI_CMD_SEED_MODE1);
		}

		if (rc) {
			pr_err("[%s] failed to send DSI_CMD_SEED_MODE1 cmds, rc=%d\n",
			       panel->name, rc);
		}

		break;

	case 2:
		if (!oplus_set_color_mode && oplus_seed_backlight && !strcmp(panel->name, "samsung AMS643YE01 dsc cmd mode panel")) {
			if (oplus_seed_backlight != oplus_seed_last_backlight) {
				rc = dsi_panel_tx_cmd_set(panel, DSI_CMD_SEED_DC_MODE2);
				oplus_seed_last_backlight = oplus_seed_backlight;
			}
		}
		else if (oplus_dc_v2_on) {
			if (g_fingerprint_mode)
				rc = dsi_panel_tx_cmd_set(panel, DSI_CMD_SEED_MODE2);
			else
				rc = dsi_panel_tx_cmd_set(panel, DSI_CMD_SEED_DC_MODE2);
		}
		else {
			rc = dsi_panel_tx_cmd_set(panel, DSI_CMD_SEED_MODE2);
		}

		if (rc) {
			pr_err("[%s] failed to send DSI_CMD_SEED_MODE2 cmds, rc=%d\n",
			       panel->name, rc);
		}

		break;

	case 3:
		rc = dsi_panel_tx_cmd_set(panel, DSI_CMD_SEED_MODE3);

		if (rc) {
			pr_err("[%s] failed to send DSI_CMD_SEED_MODE3 cmds, rc=%d\n",
			       panel->name, rc);
		}

		break;

	case 4:
		rc = dsi_panel_tx_cmd_set(panel, DSI_CMD_SEED_MODE4);

		if (rc) {
			pr_err("[%s] failed to send DSI_CMD_SEED_MODE4 cmds, rc=%d\n",
			       panel->name, rc);
		}

		break;

	default:
		rc = dsi_panel_tx_cmd_set(panel, DSI_CMD_SEED_OFF);

		if (rc) {
			pr_err("[%s] failed to send DSI_CMD_SEED_OFF cmds, rc=%d\n",
			       panel->name, rc);
		}

		pr_err("[%s] seed mode Invalid %d\n",
		       panel->name, mode);
	}

	if (oplus_set_color_mode) {
		oplus_set_color_mode = false;
	}

	return rc;
}

int dsi_panel_loading_effect_mode_unlock(struct dsi_panel *panel, int mode)
{
	int rc = 0;

	if (!dsi_panel_initialized(panel)) {
		return -EINVAL;
	}

	switch (mode) {
	case PANEL_LOADING_EFFECT_MODE1:
		if (!strcmp(panel->oplus_priv.vendor_name, "AMB670YF01") && ((panel->panel_id2 <= 0x06) || (panel->panel_id2 == 0x15))) {
			rc = dsi_panel_tx_cmd_set(panel, DSI_CMD_LOADING_EFFECT_MODE1_ID6);
			if (rc) {
				pr_err("[%s] failed to send DSI_CMD_LOADING_EFFECT_MODE1_ID6 cmds, rc=%d\n",
					panel->name, rc);
			}
		} else {
			rc = dsi_panel_tx_cmd_set(panel, DSI_CMD_LOADING_EFFECT_MODE1);
			if (rc) {
				pr_err("[%s] failed to send DSI_CMD_LOADING_EFFECT_MODE1 cmds, rc=%d\n",
					panel->name, rc);
			}
		}
		break;

	case PANEL_LOADING_EFFECT_MODE2:
		if (!strcmp(panel->oplus_priv.vendor_name, "AMB670YF01") && ((panel->panel_id2 <= 0x06) || (panel->panel_id2 == 0x15))) {
			rc = dsi_panel_tx_cmd_set(panel, DSI_CMD_LOADING_EFFECT_MODE2_ID6);
			if (rc) {
				pr_err("[%s] failed to send DSI_CMD_LOADING_EFFECT_MODE2_ID6 cmds, rc=%d\n",
					panel->name, rc);
			}
		} else {
			rc = dsi_panel_tx_cmd_set(panel, DSI_CMD_LOADING_EFFECT_MODE2);
			if (rc) {
				pr_err("[%s] failed to send DSI_CMD_LOADING_EFFECT_MODE2 cmds, rc=%d\n",
					panel->name, rc);
			}
		}
		break;

	case PANEL_LOADING_EFFECT_OFF:
		if (!strcmp(panel->oplus_priv.vendor_name, "AMB670YF01") && ((panel->panel_id2 <= 0x06) || (panel->panel_id2 == 0x15))) {
			rc = dsi_panel_tx_cmd_set(panel, DSI_CMD_LOADING_EFFECT_OFF_ID6);
			if (rc) {
				pr_err("[%s] failed to send DSI_CMD_LOADING_EFFECT_OFF_ID6 cmds, rc=%d\n",
					panel->name, rc);
			}
		} else {
			rc = dsi_panel_tx_cmd_set(panel, DSI_CMD_LOADING_EFFECT_OFF);
			if (rc) {
				pr_err("[%s] failed to send DSI_CMD_LOADING_EFFECT_OFF cmds, rc=%d\n",
					panel->name, rc);
			}
		}
		break;

	default:
		if (!strcmp(panel->oplus_priv.vendor_name, "AMB670YF01") && ((panel->panel_id2 <= 0x06) || (panel->panel_id2 == 0x15))) {
			rc = dsi_panel_tx_cmd_set(panel, DSI_CMD_LOADING_EFFECT_OFF_ID6);
			if (rc) {
				pr_err("[%s] failed to send DSI_CMD_LOADING_EFFECT_OFF_ID6 cmds, rc=%d\n",
					panel->name, rc);
			}
		} else {
			rc = dsi_panel_tx_cmd_set(panel, DSI_CMD_LOADING_EFFECT_OFF);
			if (rc) {
				pr_err("[%s] failed to send DSI_CMD_LOADING_EFFECT_OFF cmds, rc=%d\n",
					panel->name, rc);
			}
		}

		pr_err("[%s] loading effect mode Invalid %d\n",
		       panel->name, mode);
	}

	return rc;
}

int dsi_panel_seed_mode(struct dsi_panel *panel, int mode)
{
	int rc = 0;

	if (!panel) {
		pr_err("Invalid params\n");
		return -EINVAL;
	}

	//mutex_lock(&panel->panel_lock);

	if (!strcmp(panel->oplus_priv.vendor_name, "S6E3HC3") && (mode >= PANEL_LOADING_EFFECT_FLAG)) {
		rc = dsi_panel_loading_effect_mode_unlock(panel, mode);
	} else if (!strcmp(panel->oplus_priv.vendor_name, "ANA6706") && (mode >= PANEL_LOADING_EFFECT_FLAG)) {
		mode = mode - PANEL_LOADING_EFFECT_FLAG;
		rc = dsi_panel_seed_mode_unlock(panel, mode);
		seed_mode = mode;
	} else if(!strcmp(panel->oplus_priv.vendor_name, "AMB655X") && (mode >= PANEL_LOADING_EFFECT_FLAG)){
		rc = dsi_panel_loading_effect_mode_unlock(panel, mode);
	} else if(!strcmp(panel->oplus_priv.vendor_name, "AMB670YF01") && (mode >= PANEL_LOADING_EFFECT_FLAG)){
		rc = dsi_panel_loading_effect_mode_unlock(panel, mode);
	}else if(!strcmp(panel->oplus_priv.vendor_name, "JDI_ILI7807S")){
		return 0;
	} else if (!strcmp(panel->oplus_priv.vendor_name, "S6E3XA1") && (mode >= PANEL_LOADING_EFFECT_FLAG)) {
		rc = dsi_panel_loading_effect_mode_unlock(panel, mode);
	} else {
		rc = dsi_panel_seed_mode_unlock(panel, mode);
	}

	//mutex_unlock(&panel->panel_lock);
	return rc;
}

int dsi_display_seed_mode(struct dsi_display *display, int mode)
{
	int rc = 0;

	if (!display || !display->panel) {
		pr_err("Invalid params\n");
		return -EINVAL;
	}

	mutex_lock(&display->display_lock);

	/* enable the clk vote for CMD mode panels */
	if (display->config.panel_mode == DSI_OP_CMD_MODE) {
		dsi_display_clk_ctrl(display->dsi_clk_handle,
				     DSI_CORE_CLK, DSI_CLK_ON);
	}

	if ((!strcmp(display->panel->oplus_priv.vendor_name, "AMB670YF01")) ||
		(!strcmp(display->panel->oplus_priv.vendor_name, "AMB655X")) ||
		(!strcmp(display->panel->oplus_priv.vendor_name, "S6E3HC3")) ||
		(!strcmp(display->panel->oplus_priv.vendor_name, "AMS643YE01"))) {
		mutex_lock(&display->panel->panel_lock);
	}

	rc = dsi_panel_seed_mode(display->panel, mode);

	if ((!strcmp(display->panel->oplus_priv.vendor_name, "AMB670YF01")) ||
		(!strcmp(display->panel->oplus_priv.vendor_name, "AMB655X")) ||
		(!strcmp(display->panel->oplus_priv.vendor_name, "S6E3HC3")) ||
		(!strcmp(display->panel->oplus_priv.vendor_name, "AMS643YE01"))) {
		mutex_unlock(&display->panel->panel_lock);
	}

	if (rc) {
		pr_err("[%s] failed to dsi_panel_seed_or_loading_effect_on, rc=%d\n",
		       display->name, rc);
	}

	if (display->config.panel_mode == DSI_OP_CMD_MODE) {
		rc = dsi_display_clk_ctrl(display->dsi_clk_handle,
					  DSI_CORE_CLK, DSI_CLK_OFF);
	}

	mutex_unlock(&display->display_lock);
	return rc;
}

int oplus_dsi_update_seed_mode(void)
{
	struct dsi_display *display = get_main_display();
	int ret = 0;

	if (!display) {
		pr_err("failed for: %s %d\n", __func__, __LINE__);
		return -EINVAL;
	}

	ret = dsi_display_seed_mode(display, seed_mode);

	return ret;
}

int oplus_display_panel_get_seed(void *data)
{
	uint32_t *temp = data;
	printk(KERN_INFO "oplus_display_get_seed = %d\n", seed_mode);

	(*temp) = seed_mode;
	return 0;
}

int oplus_display_panel_set_seed(void *data)
{
	uint32_t *temp_save = data;
	uint32_t panel_id = (*temp_save >> 12);
	seed_mode = (*temp_save & 0x0fff);
	printk(KERN_INFO "%s oplus_display_set_seed = %d, panel_id = %d\n", __func__, seed_mode, panel_id);

	__oplus_display_set_seed(seed_mode);

	if (get_oplus_display_power_status() == OPLUS_DISPLAY_POWER_ON) {
		struct dsi_display *display = get_main_display();
		if (1 == panel_id) {
			display = get_sec_display();
		}
		if (display == NULL) {
			printk(KERN_INFO "oplus_display_set_seed and main display is null");
			return -EINVAL;
		}

		dsi_display_seed_mode(display, seed_mode);

	} else {
		printk(KERN_ERR
		       "%s oplus_display_set_seed = %d, but now display panel status is not on\n",
		       __func__, seed_mode);
	}

	return 0;
}
