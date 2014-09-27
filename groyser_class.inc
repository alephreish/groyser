<?php
class groyser {
	private $sess_id;
	var $error;
	private $db;
	private $ch;
	private $url = "http://www.cs.uky.edu/~raphael/yiddish/searchGroys.cgi";
	
	function __construct() {
		date_default_timezone_set('GMT');
		$this->db = new SQLite3(__DIR__."/groyser.sq3");
		if (!$this->db) {
			$this->error = "Failed to open the database";
			return false;
		}
		$this->db->query('CREATE TABLE IF NOT EXISTS "yi_yi" ("query" TEXT NOT NULL, "titl" TEXT NOT NULL, "taytch" TEXT NOT NULL)');
		$this->ch = curl_init();
		curl_setopt($this->ch, CURLOPT_RETURNTRANSFER, true);
		curl_setopt($this->ch, CURLOPT_USERAGENT, 'curl');
		curl_setopt($this->ch, CURLOPT_URL, $this->url);
		curl_setopt($this->ch, CURLOPT_POST, true);
		curl_setopt($this->ch, CURLOPT_CONNECTTIMEOUT, 7);
		curl_setopt($this->ch, CURLOPT_TIMEOUT, 20);
	}
	
	function stripnikkud($word) {
		return trim(str_replace(
				array(
						'ֿ',
						'ַ',
						'ָ',
						'־',
						'ּ',
						'ִ',
						'שׂ',
						'שׂ',
						'װ',
						'ױ',
						'ײ',
				),
				array(
						'',
						'',
						'',
						' ',
						'',
						'',
						'ש',
						'ש',
						'וו',
						'וי',
						'יי',
				),
				$word));
	}
	
	function translate($query) {
		$query = $this->stripnikkud($query);
		if (empty($query) or mb_substr($query, 0, 2) <> 'א') {
			return false;
		}
		if (!($result = $this->db->query("SELECT * FROM yi_yi WHERE query like '".sqlite_escape_string($query)."'"))) {
			$this->error = "Database returned error";
			return false;
		}
		$out = array();
		while (($db_reply = $result->fetchArray(SQLITE3_ASSOC))) {
			$out[$db_reply['titl']] = $db_reply['taytch'];
		}
		if (isset($out[""])) {
			$this->error = "Not found";
			return false;
		}
		if (!empty($out)) return $out;
		curl_setopt($this->ch, CURLOPT_POSTFIELDS, array('keys' => $query));
		$results = curl_exec($this->ch);
		$results = preg_replace('/<p>פֿון באַנד \d:<\/p>/u', '<hr/>', $results);
		$results = explode('<hr/>', $results);
		unset($results[0]);
		foreach ($results as $result) {
			$result = trim(str_replace(array('<p>'), array(' '), $result));
			$result = preg_replace('/\s+/', ' ', $result);
			$result = explode(' — ', $result, 2);
			if (count($result) <> 2) continue;
			$title = strip_tags($result[0]);
			if ($this->stripnikkud($title) <> $query) continue;
			$out[$title] = $result[1];
			if (!$this->db->query("INSERT INTO yi_yi (query, titl, taytch) VALUES ('".
					sqlite_escape_string($query)."', '".
					sqlite_escape_string($title)."', '".
					sqlite_escape_string($out[$title])."')")) {
				$this->error = "Database returned error";
				return false;
			}
		}
		if (empty($out)) {
			$this->error = "Not found";
			$this->db->query("INSERT INTO yi_yi (query, titl, taytch) VALUES ('".sqlite_escape_string($query)."', '', '')");
			return false;
		}
		return $out;
	}
	
	function __destruct() {
		if (is_resource($this->db)) $this->db->close();
		if (is_resource($this->ch)) curl_close($this->ch);
	}
}
?>
