<?php

/**
 * @file
 * TN datastream access override callback.
 */

/**
 * Implements callback_islandora_access_override_datastream_handler().
 */
function islandora_ip_embargo_thumbnail_access_callback($op, AbstractDatastream $datastream, $user) {
  module_load_include('inc', 'islandora_ip_embargo', 'includes/utilities');
  $restrict_access = islandora_ip_embargo_restrict_access($datastream->parent->id);
  if ($restrict_access) {
    // If we're looking for a thumbnail let it through; otherwise, deny.
    if($datastream->id == 'TN'){
        return True;
    }elseif($datastream->id == 'PREVIEW'){
        return True;
    }elseif($datastream->id == 'JPG'){
        global $base_url;
        $urlparts = preg_replace('/\?.*/', '', str_replace($base_url,'',$_SERVER['HTTP_REFERER']));
        if(substr($urlparts, -1) == '/'){
           $urlparts = substr($urlparts, 0, -1);
        }
        $pageExclude = islandora_ip_embargo_select_ignore_page($urlparts);
        if(isset($pageExclude) && !empty($pageExclude)){
            return TRUE;
        }else{
            return FALSE;
        }
    }else{
        return False;
    }
  }

  // If we found no issues we let things proceed.
  return NULL;
}
