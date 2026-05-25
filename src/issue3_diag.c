// SPDX-License-Identifier: GPL-2.0-or-later

#include <zephyr/device.h>
#include <zephyr/init.h>
#include <zephyr/input/input.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <zmk/event_manager.h>
#include <zmk/events/usb_conn_state_changed.h>
#include <zmk/usb.h>

LOG_MODULE_REGISTER(issue3_diag, CONFIG_ZMK_LOG_LEVEL);

#define ISSUE3_POINTING_NODE DT_NODELABEL(pointing_device)

static const char *usb_conn_state_name(enum zmk_usb_conn_state state) {
    switch (state) {
    case ZMK_USB_CONN_NONE:
        return "none";
    case ZMK_USB_CONN_POWERED:
        return "powered";
    case ZMK_USB_CONN_HID:
        return "hid";
    default:
        return "unknown";
    }
}

static void issue3_log_usb_state(const char *reason) {
    enum zmk_usb_conn_state state = zmk_usb_get_conn_state();

    LOG_INF("ISSUE3 usb: reason=%s uptime=%lldms status=%d conn=%s(%d) powered=%d hid_ready=%d",
            reason, (long long)k_uptime_get(), zmk_usb_get_status(), usb_conn_state_name(state),
            state, zmk_usb_is_powered(), zmk_usb_is_hid_ready());
}

static int issue3_usb_listener(const zmk_event_t *eh) {
    const struct zmk_usb_conn_state_changed *ev = as_zmk_usb_conn_state_changed(eh);

    if (ev != NULL) {
        issue3_log_usb_state("usb_conn_state_changed");
    }

    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(issue3_diag, issue3_usb_listener);
ZMK_SUBSCRIPTION(issue3_diag, zmk_usb_conn_state_changed);

#if DT_NODE_HAS_STATUS(ISSUE3_POINTING_NODE, okay)

struct issue3_input_accumulator {
    int32_t x;
    int32_t y;
    int32_t wheel;
    int32_t hwheel;
    uint32_t events;
};

static struct issue3_input_accumulator issue3_input;

static const char *input_code_name(uint8_t type, uint16_t code) {
    if (type != INPUT_EV_REL) {
        return "-";
    }

    switch (code) {
    case INPUT_REL_X:
        return "rel_x";
    case INPUT_REL_Y:
        return "rel_y";
    case INPUT_REL_WHEEL:
        return "rel_wheel";
    case INPUT_REL_HWHEEL:
        return "rel_hwheel";
    default:
        return "rel_other";
    }
}

static void issue3_input_callback(struct input_event *evt) {
    issue3_input.events++;

    if (evt->type == INPUT_EV_REL) {
        switch (evt->code) {
        case INPUT_REL_X:
            issue3_input.x += evt->value;
            break;
        case INPUT_REL_Y:
            issue3_input.y += evt->value;
            break;
        case INPUT_REL_WHEEL:
            issue3_input.wheel += evt->value;
            break;
        case INPUT_REL_HWHEEL:
            issue3_input.hwheel += evt->value;
            break;
        default:
            break;
        }
    }

    if (evt->value != 0 || evt->sync) {
        LOG_INF("ISSUE3 raw input: event=%u dev=%s type=%u code=%u(%s) value=%d sync=%u",
                issue3_input.events, evt->dev != NULL ? evt->dev->name : "none", evt->type,
                evt->code, input_code_name(evt->type, evt->code), evt->value, evt->sync);
    }

    if (evt->sync) {
        enum zmk_usb_conn_state state = zmk_usb_get_conn_state();

        LOG_INF("ISSUE3 raw sync: event=%u x=%d y=%d wheel=%d hwheel=%d usb_status=%d "
                "usb_conn=%s(%d) hid_ready=%d",
                issue3_input.events, issue3_input.x, issue3_input.y, issue3_input.wheel,
                issue3_input.hwheel, zmk_usb_get_status(), usb_conn_state_name(state), state,
                zmk_usb_is_hid_ready());

        issue3_input.x = 0;
        issue3_input.y = 0;
        issue3_input.wheel = 0;
        issue3_input.hwheel = 0;
    }
}

INPUT_CALLBACK_DEFINE(DEVICE_DT_GET(ISSUE3_POINTING_NODE), issue3_input_callback);

#endif

static int issue3_diag_init(void) {
    LOG_INF("ISSUE3 diagnostics init");
    issue3_log_usb_state("init");

#if DT_NODE_HAS_STATUS(ISSUE3_POINTING_NODE, okay)
    const struct device *dev = DEVICE_DT_GET(ISSUE3_POINTING_NODE);

    LOG_INF("ISSUE3 pointing device: name=%s ready=%d", dev->name, device_is_ready(dev));
#else
    LOG_WRN("ISSUE3 pointing device: no okay node label 'pointing_device'");
#endif

    return 0;
}

SYS_INIT(issue3_diag_init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);
