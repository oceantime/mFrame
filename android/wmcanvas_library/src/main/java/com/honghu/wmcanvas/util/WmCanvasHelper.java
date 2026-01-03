package com.honghu.wmcanvas.util;

import android.util.Log;

import com.honghu.wmcanvas.WmCanvas;
import com.honghu.wmcanvas.WmCanvas.ViewMode;
import com.honghu.wmcanvas.util.WmLog;

import org.json.JSONArray;
import org.json.JSONException;

public class WmCanvasHelper {

	/**
	 * Parse the js command args to a JsonArray Object
	 *
	 * @param command
	 * @param params
     * @return
     */
	public static JSONArray argsToJsonArray(String command, String params) {
		JSONArray args = null;
		try {
			if (params.equals("{}")) {
				args = new JSONArray();
			}
			else if (params.startsWith("\"") && params.endsWith("\"")) {
				args = new JSONArray();
				args.put(params.substring(1, params.length() - 1));
			}
			// this is hard code to optimize the performance .
			// parsing json costs much time.
			// TODO: need to find a graceful solution later.
			else if (command.equals("render") && params.startsWith("[\"")) {
				args = new JSONArray();
				args.put(params.substring(2, params.length() - 2));
			}
			else {
				args = new JSONArray(params);
			}
		} catch (JSONException e) {
			Log.w("WmCanvas", "fail to parse params:" + params);
			args = new JSONArray();
		}

		return args;
	}

	/**
	 * Try to parse a string content to WmCanvas.ViewMode
	 *
	 * @param mode
	 * @return
     */
	public static ViewMode parseViewModeString(String mode) {
		if (mode == null) {
			return WmCanvas.DEFAULT_VIEW_MODE;
		} else if (mode.equals("hybrid")) {
			return ViewMode.HYBRID_MODE;
		} else if (mode.equals("default")) {
			return WmCanvas.DEFAULT_VIEW_MODE;
		} else if (mode.equals("canvas")) {
			return ViewMode.SINGLE_CANVAS_MODE;
		} else if (mode.equals("switch")) {
			return ViewMode.SWITCH_MODE;
		} else if (mode.equals("float")) {
			return ViewMode.FLOAT_HYBRID_MODE;
		} else {
			WmLog.w("CANVAS", "cannot parse the view mode, mode:" + mode);
			return WmCanvas.DEFAULT_VIEW_MODE;
		}
	}

}

