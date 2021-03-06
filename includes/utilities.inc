<?php

/**
 * @file
 * Holds helper functions for islandora_ip_embargo.
 */

/**
 * Get all lists for table.
 *
 * This is not a general function it is used for pagination.
 *
 * @return SelectQuery
 *   The query return.
 */
function islandora_ip_embargo_get_lists() {
  $query = db_select("islandora_ip_embargo_lists");
  $query = $query->fields('islandora_ip_embargo_lists');
  $query = $query->extend('TableSort');
  $query = $query->extend('PagerDefault');
  $query = $query->limit(10);

  return $query->execute();
}

/**
 * Create a list.
 *
 * @param string $name
 *   The name of the new list.
 */
function islandora_ip_embargo_add_list($name,$embargo_msg) {
  db_insert('islandora_ip_embargo_lists')
  ->fields(array('name' => $name,'embargo_msg' => serialize($embargo_msg)))
  ->execute();
}

/**
 * Edit a list.
 *
 * @param string $name
 *   The name of the list.
 * @param int $list_identifier
 *   The identifier of the list.
 */
function islandora_ip_embargo_edit_list($name, $list_identifier, $embargo_msg) {
  db_update('islandora_ip_embargo_lists')
  ->fields(array('name' => $name,'embargo_msg' => serialize($embargo_msg)))
  ->condition('lid', $list_identifier)
  ->execute();
}

/**
 * Remove a list.
 *
 * @param int $list_identifier
 *   The identifier of the list to delete.
 */
function islandora_ip_embargo_remove_list($list_identifier) {
  db_delete('islandora_ip_embargo_lists')
    ->condition('lid', $list_identifier)
    ->execute();
}

/**
 * Get the name of a list.
 *
 * @param int $list_identifier
 *   The identifier of the list to delete.
 *
 * @return string
 *   The human readable name of the list.
 */
function islandora_ip_embargo_get_list_name($list_identifier) {
  $list = db_select('islandora_ip_embargo_lists')
    ->fields('islandora_ip_embargo_lists')
    ->condition('lid', $list_identifier)
    ->execute();
  $data = $list->fetchObject();

  return $data->name;
}

/**
 * Get the information of a list.
 *
 * @param int $list_identifier
 *   The identifier of the list to delete.
 *
 * @return string
 *   The human readable name of the list.
 */
function islandora_ip_embargo_get_list_info($list_identifier) {
  $list = db_select('islandora_ip_embargo_lists')
  ->fields('islandora_ip_embargo_lists')
  ->condition('lid', $list_identifier)
  ->execute();
  $data = $list->fetchObject();

  return $data;
}

/**
 * Get a list's information.
 *
 * @param int $list_identifier
 *   The identifier of the list.
 * @param bool $page_results
 *   TRUE to page results for display, otherwise return all results.
 *
 * @return SelectQuery
 *   The query return.
 */
function islandora_ip_embargo_get_lists_information($list_identifier, $page_results = TRUE) {
  $query = db_select("islandora_ip_embargo_lists", 'lists');
  $query->join('islandora_ip_embargo_ip_ranges', 'ranges', 'lists.lid = ranges.lid');
  $query = $query->fields('ranges');
  $query = $query->fields('lists');
  $query = $query->condition('lists.lid', $list_identifier);
  if ($page_results) {
    $query = $query->extend('TableSort');
    $query = $query->extend('PagerDefault');
    $query = $query->limit(10);
  }
  return $query->execute();
}

/**
 * Get a list's information.
 *
 * @param int $pid
 *   The identifier of the list.
 *
 * @return SelectQuery
 *   The query return.
 */
function islandora_ip_embargo_get_lists_msg($pid) {
  $query = db_select("islandora_ip_embargo_lists", 'lists');
  $query->join('islandora_ip_embargo_embargoes', 'iee', 'lists.lid = iee.lid');
  $query = $query->fields('lists');
  $query = $query->condition('iee.pid', $pid);
  $msg = $query->execute();
  $data = $msg->fetchObject();
  
  return $data->embargo_msg;
}

/**
 * Remove an IP address range.
 *
 * @param int $range_identifier
 *   The identifier of the range to delete.
 */
function islandora_ip_embargo_remove_range($range_identifier) {
  db_delete('islandora_ip_embargo_ip_ranges')
    ->condition('rid', $range_identifier)
    ->execute();
}

/**
 * Create an IP address range on a list.
 *
 * @param int $list_identifier
 *   The identifier of the list.
 * @param string $low_end
 *   The low end of the IP range.
 * @param string $high_end
 *   The high end of the IP range.
 */
function islandora_ip_embargo_add_ip_range($list_identifier, $low_end, $high_end) {
  db_insert('islandora_ip_embargo_ip_ranges')
    ->fields(array(
      'low_end' => $low_end,
      'high_end' => $high_end,
      'lid' => $list_identifier,
    ))
    ->execute();
}

/**
 * Get an object's embargo information.
 *
 * @param string $islandora_object_id
 *   The PID of the object to get the embargo for.
 *
 * @return SelectQuery
 *   The query return.
 */
function islandora_ip_embargo_get_embargo($islandora_object_id) {
  $embargoes = db_select('islandora_ip_embargo_embargoes')
    ->fields('islandora_ip_embargo_embargoes')
    ->condition('pid', $islandora_object_id)
    ->execute();

  return $embargoes;
}

/**
 * Gets embargoes associated with a list.
 *
 * @param string $list_identifier
 *   The id of the list to find embargoes for.
 *
 * @return SelectQuery
 *   The query return.
 */
function islandora_ip_embargo_get_embargos($list_identifier) {
  $embargoes = db_select('islandora_ip_embargo_embargoes')
    ->fields('islandora_ip_embargo_embargoes')
    ->condition('lid', $list_identifier)
    ->execute();

  return $embargoes;
}

/**
 * Update an object's embargo information.
 *
 * @param string $islandora_object_id
 *   The PID of the object to update the embargo for.
 * @param int $list_id
 *   The IP address range list to use for the embargo.
 * @param int $expiry
 *   The timestamp to stop the embargo at.
 */
function islandora_ip_embargo_update_embargo($islandora_object_id, $list_id, $expiry = NULL) {
  db_update('islandora_ip_embargo_embargoes')
    ->fields(array(
      'pid' => $islandora_object_id,
      'lid' => $list_id,
      'expiry' => $expiry,
    ))
    ->condition('pid', $islandora_object_id)
    ->execute();
}

/**
 * Set an object's embargo information.
 *
 * @param string $islandora_object_id
 *   The PID of the object to set the embargo for.
 * @param int $list_id
 *   The IP address range list to use for the embargo.
 * @param int $expiry
 *   The timestamp to stop the embargo at.
 */
function islandora_ip_embargo_set_embargo($islandora_object_id, $list_id, $expiry = NULL) {
  db_insert('islandora_ip_embargo_embargoes')
    ->fields(array(
      'pid' => $islandora_object_id,
      'lid' => $list_id,
      'expiry' => $expiry,
    ))
    ->execute();
  module_invoke_all('islandora_ip_embargo_set', $islandora_object_id, array(
    'lid' => $list_id,
    'expiry' => $expiry,
  ));
}

/**
 * Remove an object's embargo information.
 *
 * @param string $islandora_object_id
 *   The PID of the object to remove the embargo from.
 */
function islandora_ip_embargo_remove_embargo($islandora_object_id) {
  db_delete('islandora_ip_embargo_embargoes')
    ->condition('pid', $islandora_object_id)
    ->execute();
  module_invoke_all('islandora_ip_embargo_lifted', $islandora_object_id);
}

/**
 * Will check if the current user's request IP is in an indicated range.
 *
 * @param string $low
 *   The low end bound of the IP address range.
 * @param string $high
 *   The high end bound of the IP address range.
 */
function islandora_ip_embargo_ip_in_range($low, $high) {
  $comparable_address = ip2long(ip_address());
  $comparable_high = ip2long($high);
  $comparable_low = ip2long($low);
  if ($comparable_address >= $comparable_low && $comparable_address <= $comparable_high) {
    return TRUE;
  }
  return FALSE;
}

/**
 * Remove all embargoes if the expiry timestamp has passed.
 *
 * @param int $time_stamp
 *   The timestamp to judge embargo expiries by.
 */
function islandora_ip_embargo_cron_cleanup($time_stamp) {
  $num_deleted = db_delete('islandora_ip_embargo_embargoes')
    ->condition('expiry', $time_stamp, '<=')
    ->execute();

  watchdog(
    'Cron Job End',
    "The ip embargo cron job has ended, @num_deleted embargos have been removed.",
    array('@num_deleted' => $num_deleted)
  );
}

/**
 * This will send islandora_ip_embargo_to_be_lifted events via rules module.
 *
 * @param int $days_before
 *   How many days before an embargo is lifted to send the event.
 */
function islandora_ip_embargo_send_embargo_lift_events($days_before) {
  module_load_include('inc', 'islandora_ip_embargo', 'includes/batch');
  $event_threshold = time() + $days_before * 24 * 60 * 60;

  // If rules is enabled trigger a batch of events for embargoes being lifted.
  if (module_exists('rules')) {
    batch_set(array(
      'operations' => array(
        array(
          'islandora_ip_embargo_embargo_lift_event_batch',
          array($event_threshold),
        ),
      ),
      'finished' => 'islandora_ip_embargo_lift_event_batch_finished',
      'title' => t('Sending out embargo lift events.'),
      'init_message' => t('Event batch starting.'),
      'progress_message' => t('Processed @current out of @total.'),
      'error_message' => t('Event batch has encountered an error.'),
      'file' => drupal_get_path('module', 'islandora_ip_embargo') . '/includes/batch.inc',
    ));

    // This is a hack around the broken batch API.
    // https://drupal.org/node/638712#comment-2289138
    $batch =& batch_get();
    $batch['progressive'] = FALSE;

    batch_process();
  }
}

/**
 * This will get the number of embargoes that expire after a given time.
 *
 * Only counts those that have not had their event triggered.
 *
 * @param int $event_threshold
 *   Timestamp of upper bound.
 *
 * @return int
 *   The number of embargoes.
 */
function islandora_ip_embargo_count_embargoes_before_timestamp($event_threshold) {
  return db_select('islandora_ip_embargo_embargoes')
    ->fields('islandora_ip_embargo_embargoes')
    ->condition('expiry', $event_threshold, '<=')
    ->condition('expiry_event_triggered', 0, '=')
    ->countQuery()
    ->execute()
    ->fetchField();
}

/**
 * This function will get the list of embargoes that expire after a given time.
 *
 * Only returns those that have not had their event triggered.
 *
 * @param int $event_threshold
 *   Timestamp of upper bound.
 * @param int $offset
 *   The offset for the query.
 * @param int $limit
 *   The limit for the query.
 *
 * @return object
 *   The results object from the query.
 */
function islandora_ip_embargo_get_embargoes_before_timestamp($event_threshold, $offset, $limit) {
  return db_select('islandora_ip_embargo_embargoes')
    ->fields('islandora_ip_embargo_embargoes')
    ->condition('expiry', $event_threshold, '<=')
    ->condition('expiry_event_triggered', 0, '=')
    ->orderBy('pid', 'ASC')
    ->range($offset, $limit)
    ->execute();
}

/**
 * Updates the table to indicated the objects' embargo lift events were sent.
 *
 * @param int $event_threshold
 *   Timestamp of upper bound for objects to set the triggered state on.
 *
 * @return object
 *   The results object from the update.
 */
function islandora_ip_embargo_set_event_triggered($event_threshold) {
  return db_update('islandora_ip_embargo_embargoes')
    ->fields(array('expiry_event_triggered' => 1))
    ->condition('expiry', $event_threshold, '<=')
    ->condition('expiry_event_triggered', 0, '=')
    ->execute();
}

/**
 * Retrieve all objects effected by embargos, using pageination.
 *
 * @param int $limit
 *   The limit to set on the query.
 *
 * @return array
 *   The list of embargoed objects.
 */
function islandora_ip_embargo_get_all_embargo_objects($limit = 10) {
  $query = db_select('islandora_ip_embargo_embargoes');
  $query = $query->extend('TableSort');
  $query = $query->extend('PagerDefault');
  $query = $query->fields('islandora_ip_embargo_embargoes');
  $query = $query->limit($limit);
  $embargoes = $query->execute();

  return $embargoes;
}

/**
 * Checks if we should restrict access to the object based on ip embargoes.
 *
 * @param string $pid
 *   The PID of the object to check.
 *
 * @return bool
 *   True if one should restrict access. False if one should not.
 */
function islandora_ip_embargo_restrict_access($pid) {
  $open_access = FALSE;

  $embargo_result = islandora_ip_embargo_get_embargo($pid);
  if ($embargo_result->rowCount()) {
    $embargo_info = $embargo_result->fetchObject();
    $list_results = islandora_ip_embargo_get_lists_information($embargo_info->lid, FALSE);
    if ($list_results->rowCount()) {
      while ($list_info = $list_results->fetchObject()) {
        if (islandora_ip_embargo_ip_in_range($list_info->low_end, $list_info->high_end)) {
          $open_access = TRUE;
        }
      }
    }
    else {
      $open_access = TRUE;
    }
  }
  else {
    $open_access = TRUE;
  }
  return !$open_access;
}

/**
 * Select all exception page id
 * @param string $pid
 *   The PID of the object to check.
 *
 * @return array
 *   List of page id will be exclude in IP embargo.
 */
function islandora_ip_embargo_select_ignore_page_list() {
  $query = db_select("islandora_ip_embargo_ignore_page_list");
  $query = $query->fields('islandora_ip_embargo_ignore_page_list');
  $query = $query->extend('TableSort');
  $query = $query->extend('PagerDefault');
  $query = $query->limit(50);

  return $query->execute()->fetchAll(PDO::FETCH_ASSOC);
}

/**
 * Select exception page with url
 * @param string $url
 *   URL of page will be exclude in embargo
 *
 * @return array
 *   page will be exclude in IP embargo.
 */
function islandora_ip_embargo_select_ignore_page($url) {
  $result = db_select("islandora_ip_embargo_ignore_page_list")
          ->fields('islandora_ip_embargo_ignore_page_list')
          ->condition('alias',$url ,'=')
          ->execute()->fetchAll(PDO::FETCH_ASSOC);

  return $result;
}

/**
 * Select exception page with url
 * @param string $url
 *   URL of page will be exclude in embargo
 *
 * @return array
 *   page will be exclude in IP embargo.
 */
function islandora_ip_embargo_select_ignore_page_with_nid($nid) {
  $result = db_select("islandora_ip_embargo_ignore_page_list")
          ->fields('islandora_ip_embargo_ignore_page_list')
          ->condition('node_id',$nid ,'=')
          ->execute()->fetchAll(PDO::FETCH_ASSOC);

  return $result;
}
/**
 * Select exception page id with filter
 * @param string $pid
 *   The PID of the object to check.
 *
 * @return array
 *   List of page will be exclude in IP embargo
 */
function islandora_ip_embargo_filter_ignore_page_list($filter_list = NULL) {
  $params = array();
  if(!empty($filter_list)){
    foreach($filter_list as $value){
      $params[] = trim($value);
    }
  }
  $query = db_select("islandora_ip_embargo_ignore_page_list");
  $query = $query->fields('islandora_ip_embargo_ignore_page_list');
  $query = $query->condition('alias',$params ,'IN');
  $query = $query->extend('TableSort');
  $query = $query->extend('PagerDefault');
  $query = $query->limit(50);

  return $query->execute()->fetchAll(PDO::FETCH_ASSOC);
}

function islandora_ip_embargo_add_ignore_page_list($list = array()) {
    db_insert('islandora_ip_embargo_ignore_page_list')
        ->fields(array('node_id' => $list['node_id'],'alias' => $list['alias'],'ignore_datastream' => NULL))
        ->execute();
}

function islandora_ip_embargo_remove_ignore_page_list($id) {
    db_delete('islandora_ip_embargo_ignore_page_list')
    ->condition('id', $id)
    ->execute();
}